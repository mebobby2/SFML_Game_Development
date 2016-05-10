//
//  World.cpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "World.hpp"
#include "ResourcePath.hpp"

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
    
    sf::Vector2f position = mPlayerAircraft->getPosition();
    sf::Vector2f velocity = mPlayerAircraft->getVelocity();
    if (position.x <= mWorldBounds.left + 150.f || position.x >= mWorldBounds.left + mWorldBounds.width - 150.f) {
        velocity.x = -velocity.x;
        mPlayerAircraft->setPosition(velocity);
    }
    mSceneGraph.update(dt);
}

void World::draw()
{
    mWindow.setView(mWorldView);
    mWindow.draw(mSceneGraph);
}

void World::loadTextures()
{
    mTextures.load(Textures::Eagle, resourcePath() + "Eagle.png");
    mTextures.load(Textures::Rapter, resourcePath() + "Raptor.png");
    mTextures.load(Textures::Desert, resourcePath() + "Desert.png");
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
    mPlayerAircraft->setVelocity(40.f, mScrollSpeed);
    mSceneLayers[Air]->attachChild(std::move(leader));
    
    // Add two escorting aircrafts, placed relatively to main plane
    std::unique_ptr<Aircraft> leftEscort(new Aircraft(Aircraft::Rapter, mTextures));
    leftEscort->setPosition(-80.f, 50.f);
    mPlayerAircraft->attachChild(std::move(leftEscort));
    
    std::unique_ptr<Aircraft> rightEscort(new Aircraft(Aircraft::Rapter, mTextures));
    rightEscort->setPosition(80.f, 50.f);
    mPlayerAircraft->attachChild(std::move(rightEscort));
    
}

