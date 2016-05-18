//
//  World.cpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "World.hpp"

#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>

World::World(sf::RenderWindow& window)
: mWindow(window)
, mWorldView(window.getDefaultView())
, mTextures()
, mSceneGraph()
, mSceneLayers()
, mWorldBounds(0.f, 0.f, mWorldView.getSize().x, 2000.f)
, mSpawnPosition(mWorldView.getSize().x / 2.f, mWorldBounds.height - mWorldView.getSize().y / 2.f)
, mScrollSpeed(-50.f)
, mPlayerAircraft(nullptr)
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
    
    while (!mCommandQueue.isEmpty())
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    adaptPlayerVelocity();
    
    mSceneGraph.update(dt);
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

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, "Eagle.png");
    mTextures.load(Textures::Rapter, "Raptor.png");
    mTextures.load(Textures::Desert, "Desert.png");
}

void World::buildScene()
{
    // Set up the layers
    for (std::size_t i = 0; i < LayerCount; i++)
    {
        SceneNode::Ptr layer(new SceneNode());
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
    std::unique_ptr<Aircraft> leader(new Aircraft(Aircraft::Eagle, mTextures));
    mPlayerAircraft = leader.get();
    mPlayerAircraft->setPosition(mSpawnPosition);
    mSceneLayers[Air]->attachChild(std::move(leader));
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

void World::adaptPlayerPosition()
{
    sf::FloatRect viewBounds(mWorldView.getCenter() - mWorldView.getSize() / 2.f, mWorldView.getSize());
    const float borderDistance = 40.f;
    
    sf::Vector2f position = mPlayerAircraft->getPosition();
    position.x = std::max(position.x, viewBounds.left + borderDistance);
    position.x = std::min(position.x, viewBounds.left + viewBounds.width - borderDistance);
    position.y = std::max(position.y, viewBounds.top + borderDistance);
    position.y = std::min(position.y, viewBounds.top + viewBounds.height - borderDistance);
    mPlayerAircraft->setPosition(position);
}

