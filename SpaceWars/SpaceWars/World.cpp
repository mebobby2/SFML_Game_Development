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
#include "ParticleNode.hpp"
#include "SoundNode.hpp"
#include "NetworkNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>
#include <algorithm>
#include <limits>

#include <SFML/Graphics/RenderWindow.hpp>

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts, SoundPlayer& sounds, bool networked)
: mTarget(outputTarget)
, mSceneTexture()
, mWorldView(outputTarget.getDefaultView())
, mTextures()
, mFonts(fonts)
, mSounds(sounds)
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 5000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mScrollSpeedCompensation(1.f)
, mPlayerAircrafts()
, mEnemySpawnPoints()
, mActiveEnemies()
, mNetworkedWorld(networked)
, mNetworkNode(nullptr)
, mFinishSprite(nullptr)
{
    mSceneTexture.create(mTarget.getSize().x, mTarget.getSize().y);
    
    loadTextures();
    buildScene();
    
    mWorldView.setCenter(mSpawnPosition);
}

void World::setWorldScrollCompensation(float compensation)
{
    mScrollSpeedCompensation = compensation;
}

void World::update(sf::Time dt)
{
    mWorldView.move(0.f, mScrollSpeed * dt.asSeconds() * mScrollSpeedCompensation);
    
    // Need to reset the plane's velocity in each update frame so the plane appears to be moving at the constant speed.
    // If we do not do this, then in each update frame, the plane's new velocity will be the old velocity (in the
    // previous frame) plus the current velocity, so it looks like the plane is accelerating!
    for (Aircraft* a : mPlayerAircrafts)
        a->setVelocity(0.f, 0.f);
    
    destroyEntitiesOutsideView();
    guideMissiles();
    
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    adaptPlayerVelocity();
    
    handleCollisions();
    
    auto firstToRemove = std::remove_if(mPlayerAircrafts.begin(), mPlayerAircrafts.end(), std::mem_fn(&Aircraft::isMarkedForRemoval));
    mPlayerAircrafts.erase(firstToRemove, mPlayerAircrafts.end());
    
    mSceneGraph.removeWrecks();
    spawnEnemies();
    
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
    
    updateSounds();
}

void World::draw()
{
    if (PostEffect::isSupported())
    {
        mSceneTexture.clear();
        mSceneTexture.setView(mWorldView);
        mSceneTexture.draw(mSceneGraph);
        mSceneTexture.display();
        mBloomEffect.apply(mSceneTexture, mTarget);
    }
    else
    {
        mTarget.setView(mWorldView);
        mTarget.draw(mSceneGraph);
    }
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

Aircraft* World::getAircraft(int identifier) const
{
    for (Aircraft* a : mPlayerAircrafts) {
        if (a->getIdentifier() == identifier)
            return a;
    }
    return nullptr;
}

void World::removeAircraft(int identifier)
{
    Aircraft* aircraft = getAircraft(identifier);
    if (aircraft)
    {
        aircraft->destroy();
        mPlayerAircrafts.erase(std::find(mPlayerAircrafts.begin(), mPlayerAircrafts.end(), aircraft));
    }
}

Aircraft* World::addAircraft(int identifier)
{
    std::unique_ptr<Aircraft> player(new Aircraft(Aircraft::Eagle, mTextures, mFonts));
    player->setPosition(mWorldView.getCenter());
    player->setIdentifier(identifier);
    
    mPlayerAircrafts.push_back(player.get());
    mSceneLayers[UpperAir]->attachChild(std::move(player));
    return mPlayerAircrafts.back();
}

void World::createPickup(sf::Vector2f position, Pickup::Type type)
{
    std::unique_ptr<Pickup> pickup(new Pickup(type, mTextures));
    pickup->setPosition(position);
    pickup->setVelocity(0.f, 1.f);
    mSceneLayers[UpperAir]->attachChild(std::move(pickup));
}

bool World::pollGameAction(GameActions::Action &out)
{
    return mNetworkNode->pollGameAction(out);
}

void World::setCurrentBattleFieldPosition(float lineY)
{
    mWorldView.setCenter(mWorldView.getCenter().x, lineY - mWorldView.getSize().y/2);
    mSpawnPosition.y = mWorldBounds.height;
}

void World::setWorldHeight(float height)
{
    mWorldBounds.height = height;
}

bool World::hasAlivePlayer() const
{
    return mPlayerAircrafts.size() > 0;
}

bool World::hasPlayerReachedEnd() const
{
    if (Aircraft* aircraft = getAircraft(1))
        return !mWorldBounds.contains(aircraft->getPosition());
    else
        return false;
}

void World::loadTextures()
{
    mTextures.load(Textures::Entities, "Entities.png");
    mTextures.load(Textures::Jungle, "Jungle.png");
    mTextures.load(Textures::Explosion, "Explosion.png");
    mTextures.load(Textures::Particle, "Particle.png");
    mTextures.load(Textures::FinishLine, "FinishLine.png");
}

void World::adaptPlayerPosition()
{
    sf::FloatRect viewBounds = getViewBounds();
    const float borderDistance = 40.f;
    
    for (Aircraft* aircraft : mPlayerAircrafts)
    {
        sf::Vector2f position = aircraft->getPosition();
        position.x = std::max(position.x, viewBounds.left + borderDistance);
        position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
        position.y = std::max(position.y, viewBounds.top + borderDistance);
        position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
        aircraft->setPosition(position);
    }
}

void World::adaptPlayerVelocity()
{
    for (Aircraft* aircraft : mPlayerAircrafts)
    {
        sf::Vector2f velocity = aircraft->getVelocity();
        
        // If moving diagonally, reduce velocity (to have always same velocity)
        if (velocity.x != 0.f && velocity.y != 0.f)
            aircraft->setVelocity(velocity / std::sqrt(2.f));
        
        // Add the scrollSpeed to the plane so the plane remains on the same position of the screen.
        // If we don't do this:
        //   If the player is not moving the arrow keys and then plane is still, it will scroll to the bottom
        //   of the screen and it will look like the plane is moving WITH the dessert.
        // If we do this:
        //   And the player is not inputting arrow keys, then the plane remains in the same position of the screen,
        //   and this gives the illusion that the plan is flying through the air.
        aircraft->accelerate(0.f, mScrollSpeed);
    }
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
            player.playLocalSound(mCommandQueue, SoundEffect::CollectPickup);
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

void World::updateSounds()
{
    sf::Vector2f listenerPosition;
    
    if (mPlayerAircrafts.empty())
    {
        listenerPosition = mWorldView.getCenter();
    }
    else
    {
        for (Aircraft* aircraft : mPlayerAircrafts)
            listenerPosition += aircraft->getWorldPosition();
        
        listenerPosition /= static_cast<float>(mPlayerAircrafts.size());
    }
    
    mSounds.setListenerPosition(listenerPosition);
    
    mSounds.removeStoppedSounds();
}

void World::buildScene()
{
    // Set up the layers
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        Category::Type category = (i == LowerAir) ? Category::SceneAirLayer : Category::None;
        
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayers[i] = layer.get();
        
        mSceneGraph.attachChild(std::move(layer));
    }
    
    // Prepare the tiled background
    sf::Texture& jungleTexture = mTextures.get(Textures::Jungle);
    jungleTexture.setRepeated(true);
    
    float viewHeight = mWorldView.getSize().y;
    sf::IntRect textureRect(mWorldBounds);
    textureRect.height += static_cast<int>(viewHeight);
    
    // Add backgound sprite to the scene
    std::unique_ptr<SpriteNode> jungleSprite(new SpriteNode(jungleTexture, textureRect));
    jungleSprite->setPosition(mWorldBounds.left, mWorldBounds.top - viewHeight);
    mSceneLayers[Background]->attachChild(std::move(jungleSprite));
    
    // Add the finish line to the scene
    sf::Texture& finishTexture = mTextures.get(Textures::FinishLine);
    std::unique_ptr<SpriteNode> finishSprite(new SpriteNode(finishTexture));
    finishSprite->setPosition(0.f, -76.f);
    mSceneLayers[Background]->attachChild(std::move(finishSprite));
    
    // Add particle node to the scene
    std::unique_ptr<ParticleNode> smokeNode(new ParticleNode(Particle::Smoke, mTextures));
    mSceneLayers[LowerAir]->attachChild(std::move(smokeNode));
    
    // Add propellant particle node to the scene
    std::unique_ptr<ParticleNode> propellantNode(new ParticleNode(Particle::Propellant, mTextures));
    mSceneLayers[LowerAir]->attachChild(std::move(propellantNode));
    
    // Add sound effect node
    std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
    mSceneGraph.attachChild(std::move(soundNode));
    
    // Add netowkr node, if neccessary
    if (mNetworkedWorld)
    {
        std::unique_ptr<NetworkNode> networkNode(new NetworkNode());
        mNetworkNode = networkNode.get();
        mSceneGraph.attachChild(std::move(networkNode));
    }
    
    addEnemies();
}

void World::addEnemies()
{    
    addEnemy(Aircraft::Raptor,    0.f,  500.f);
    addEnemy(Aircraft::Raptor,    0.f, 1000.f);
    addEnemy(Aircraft::Raptor, +100.f, 1150.f);
    addEnemy(Aircraft::Raptor, -100.f, 1150.f);
    addEnemy(Aircraft::Avenger,  70.f, 1500.f);
    addEnemy(Aircraft::Avenger, -70.f, 1500.f);
    addEnemy(Aircraft::Avenger, -70.f, 1710.f);
    addEnemy(Aircraft::Avenger,  70.f, 1700.f);
    addEnemy(Aircraft::Avenger,  30.f, 1850.f);
    addEnemy(Aircraft::Raptor,  300.f, 2200.f);
    addEnemy(Aircraft::Raptor, -300.f, 2200.f);
    addEnemy(Aircraft::Raptor,    0.f, 2200.f);
    addEnemy(Aircraft::Raptor,    0.f, 2500.f);
    addEnemy(Aircraft::Avenger,-300.f, 2700.f);
    addEnemy(Aircraft::Avenger,-300.f, 2700.f);
    addEnemy(Aircraft::Raptor,    0.f, 3000.f);
    addEnemy(Aircraft::Raptor,  250.f, 3250.f);
    addEnemy(Aircraft::Raptor, -250.f, 3250.f);
    addEnemy(Aircraft::Avenger,   0.f, 3500.f);
    addEnemy(Aircraft::Avenger,   0.f, 3700.f);
    addEnemy(Aircraft::Raptor,    0.f, 3800.f);
    addEnemy(Aircraft::Avenger,   0.f, 4000.f);
    addEnemy(Aircraft::Avenger,-200.f, 4200.f);
    addEnemy(Aircraft::Raptor,  200.f, 4200.f);
    addEnemy(Aircraft::Raptor,    0.f, 4400.f);
    
    sortEnemies();
}

void World::sortEnemies()
{
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
        if (mNetworkedWorld) enemy->disablePickups();
        
        mSceneLayers[UpperAir]->attachChild(std::move(enemy));
        
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


















