//
//  Game.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 29/04/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "ResourcePath.hpp"
#include "Game.hpp"
#include "StringHelpers.hpp"

#include <SFML/Window/Event.hpp>

const sf::Time Game::TimePerFrame = sf::seconds(1.f/60.f);

Game::Game()
: mWindow(sf::VideoMode(640, 480), "SpaceWars!", sf::Style::Close)
, mWorld(mWindow)
, mFont()
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{
    mFont.loadFromFile(resourcePath() + "Sansation.ttf");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (mWindow.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        //Read about fixed time steps here: http://gafferongames.com/game-physics/fix-your-timestep/
        while (timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            processInput();
            update(TimePerFrame);
        }
        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processInput()
{
    CommandQueue& commands = mWorld.getCommandQueue();
    
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        mPlayer.handleEvent(event, commands);
        
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
    
    mPlayer.handleRealtimeInput(commands);
}

void Game::update(sf::Time elapsedTime)
{
    mWorld.update(elapsedTime);
}

void Game::updateStatistics(sf::Time elapsedTime)
{
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames += 1;
    
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        mStatisticsText.setString(
                                  "Frames / Second = " + toString(mStatisticsNumFrames) + "\n" +
                                  "Time / Update = " + toString(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
							 
        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Game::render()
{
    mWindow.clear();
    mWorld.draw();
    
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    mWindow.display();
}
