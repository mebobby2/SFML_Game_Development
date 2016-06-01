//
//  World.cpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "World.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "TextNode.hpp"

#include <cmath>
#include <algorithm>
#include <limits>
#include <set>

#include <SFML/Graphics/RenderWindow.hpp>


World::World(sf::RenderWindow& window, FontHolder& fonts)
: mWindow(window)
, mFonts(fonts)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
, mEnemySpawnPoints()
, mActiveEnemies()
{
    loadTextures();
    buildScene();
    
    mWorldView.setCenter(mSpawnPosition);
}

void World::update(sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds());
    
    // Need to reset the plane's velocity in each update frame so the plane appears to be moving at the constant speed.
    // If we do not do this, then in each update frame, the plane's new velocity will be the old velocity (in the
    // previous frame) plus the current velocity, so it looks like the plane is accelerating!
    mPlayerAircraft->setVelocity(0.f, 0.f);
    
    destroyEntitiesOutsideView();
    guideMissiles();
    
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    adaptPlayerVelocity();
    
    handleCollisions();
    
    mSceneGraph.removeWrecks();
    spawnEnemies();
    
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

bool World::hasAlivePlayer() const
{
    return !mPlayerAircraft->isMarkedForRemoval();
}

bool World::hasPlayerReachedEnd() const
{
    return !mWorldBounds.contains(mPlayerAircraft->getPosition());
}

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, "Eagle.png");
    mTextures.load(Textures::Rapter, "Raptor.png");
    mTextures.load(Textures::Desert, "Avenger.png");
    mTextures.load(Textures::Desert, "Desert.png");
    
    mTextures.load(Textures::Desert, "Bullet.png");
    mTextures.load(Textures::Desert, "Missile.png");
    
    mTextures.load(Textures::Desert, "HealthRefill.png");
    mTextures.load(Textures::Desert, "MissileRefill.png");
    mTextures.load(Textures::Desert, "FireSpread.png");
    mTextures.load(Textures::Desert, "FireRate.png");
}

void World::adaptPlayerPosition()
{
    sf::FloatRect viewBounds = getViewBounds();
    const float borderDistance = 40.f;
    
    sf::Vector2f position = mPlayerAircraft->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();
    
    // If moving diagonally, reduce velocity (to have always same velocity)
    if (velocity.x != 0.f && velocity.y != 0.f)
        mPlayerAircraft->setVelocity(velocity / std::sqrt(2.f));
    
    // Add the scrollSpeed to the plane so the plane remains on the same position of the screen.
    // If we don't do this:
    //   If the player is not moving the arrow keys and then plane is still, it will scroll to the bottom
    //   of the screen and it will look like the plane is moving WITH the dessert.
    // If we do this:
    //   And the player is not inputting arrow keys, then the plane remains in the same position of the screen,
    //   and this gives the illusion that the plan is flying through the air.
    mPlayerAircraft->accelerate(0.f, mScrollSpeed);
}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1, Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();
    
    if (type1 & category1 && type2 & category2) {
        return true;
    }
    else if (type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else
    {
        return false;
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);
    
    for (SceneNode::Pair pair : collisionPairs)
    {
        if (matchesCategories(pair, Category::PlayerAircraft, Category::EnemyAircraft))
        {
            auto& player = static_cast<Aircraft&>(*pair.first);
            auto& enemy = static_cast<Aircraft&>(*pair.second);
            
            player.damage(enemy.getHitpoints());
            enemy.destroy();
        }
        
        else if (matchesCategories(pair, Category::PlayerAircraft, Category::Pickup))
        {
            auto& player = static_cast<Aircraft&>(*pair.first);
            auto& pickup = static_cast<Pickup&>(*pair.second);
            
            pickup.apply(player);
            pickup.destroy();
        }
        
        else if (matchesCategories(pair, Category::EnemyAircraft, Category::AlliedProjectile)
                 || matchesCategories(pair, Category::PlayerAircraft, Category::EnemyProjectile))
        {
            auto& aircraft = static_cast<Aircraft&>(*pair.first);
            auto& projectile = static_cast<Projectile&>(*pair.second);
            
            aircraft.damage(projectile.getDamage());
            projectile.destroy();
        }
    }
    
}

void World::buildScene()
{
    // Set up the layers
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        Category::Type category = (i == Air) ? Category::SceneAirLayer : Category::None;
        
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        
        mSceneGraph.attachChild(std::move(layer));
    }
    
    // Set up the desert background
    sf::Texture& texture = mTextures.get(Textures::Desert);
    sf::IntRect textureRect(mWorldBounds);
    texture.setRepeated(true);
    
    std::unique_ptr<SpriteNode> backgroundSprite(new SpriteNode(texture, textureRect));
    backgroundSprite->setPosition(mWorldBounds.left, mWorldBounds.top);
    mSceneLayers[Background]->attachChild(std::move(backgroundSprite));
    
    // Add Player's Aircraft
    std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
    mPlayerAircraft = player.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mSceneLayers[Air]->attachChild(std::move(player));
    
    addEnemies();
}

void World::addEnemies()
{
    addEnemy(Aircraft::Rapter, 0.f, 500.f);
    addEnemy(Aircraft::Rapter, 0.f, 1000.f);
    addEnemy(Aircraft::Rapter, +100.f, 1100.f);
    addEnemy(Aircraft::Rapter, -100.f, 1100.f);
    addEnemy(Aircraft::Rapter, -70.f, 1400.f);
    addEnemy(Aircraft::Rapter, -70.f, 1600.f);
    addEnemy(Aircraft::Rapter, 70.f, 1400.f);
    addEnemy(Aircraft::Rapter, 70.f, 1600.f);
    
    std::sort(mEnemySpawnPoints.begin(), mEnemySpawnPoints.end(), [] (SpawnPoint lhs, SpawnPoint rhs)
              {
                  return lhs.y < rhs.y;
              });
}

void World::addEnemy(Aircraft::Type type, float relX, float relY)
{
    SpawnPoint spawn(type, mSpawnPosition.x + relX, mSpawnPosition.y - relY);
    mEnemySpawnPoints.push_back(spawn);
}

void World::spawnEnemies()
{
    while (!mEnemySpawnPoints.empty() && mEnemySpawnPoints.back().y > getBattlefieldBounds().top)
    {
        SpawnPoint spawn = mEnemySpawnPoints.back();
        
        std::unique_ptr<Aircraft> enemy(new Aircraft(spawn.type, mTextures, mFonts));
        enemy->setPosition(spawn.x, spawn.y);
        enemy->setRotation(180.f);
        
        mSceneLayers[Air]->attachChild(std::move(enemy));
        
        mEnemySpawnPoints.pop_back();
    }
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category = Category::Projectile | Category::EnemyAircraft;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time)
                                           {
                                               if (!getBattlefieldBounds().intersects(e.getBoundingRect()))
                                                   e.destroy();
                                           });
    mCommandQueue.push(command);
}

void World::guideMissiles()
{
    Command enemyCollector;
    enemyCollector.category = Category::EnemyAircraft;
    enemyCollector.action = derivedAction<Aircraft>([this] (Aircraft& enemy, sf::Time)
                                                    {
                                                        if (!enemy.isDestroyed())
                                                            mActiveEnemies.push_back(&enemy);
                                                    });
    
    Command missileGuider;
    missileGuider.category = Category::AlliedProjectile;
    missileGuider.action = derivedAction<Projectile>([this] (Projectile& missle, sf::Time)
                                                     {
                                                         if (!missle.isGuided())
                                                             return;
                                                         
                                                         float minDistance = std::numeric_limits<float>::max();
                                                         Aircraft* closestEnemy = nullptr;
                                                         
                                                         for (Aircraft* enemy : mActiveEnemies) {
                                                             float enemyDistance = distance(missle, *enemy);
                                                             
                                                             if (enemyDistance < minDistance)
                                                             {
                                                                 closestEnemy = enemy;
                                                                 minDistance = enemyDistance;
                                                             }
                                                         }
                                                         
                                                         if (closestEnemy)
                                                             missle.guideTowards(closestEnemy->getWorldPosition());
                                                     });
    
    mCommandQueue.push(enemyCollector);
    mCommandQueue.push(missileGuider);
    mActiveEnemies.clear();
}

sf::FloatRect World::getViewBounds() const
{
    return sf::FloatRect(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
}

sf::FloatRect World::getBattlefieldBounds() const
{
    sf::FloatRect bounds = getViewBounds();
    bounds.top -= 100.f;
    bounds.height += 100.f;
    return bounds;
}
