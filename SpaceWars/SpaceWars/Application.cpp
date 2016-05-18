//
//  Game.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 29/04/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "Application.hpp"
#include "Utility.hpp"
#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "TitleState.hpp"
#include "GameState.hpp"
#include "MenuState.hpp"
#include "PauseState.hpp"
#include "LoadingState.hpp"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
: mWindow(sf::VideoMode(640, 480), "SpaceWars!", sf::Style::Close)
, mTextures()
, mFonts()
, mPlayer()
, mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer))
, mStatisticsText()
, mStatisticsUpdateTime()
, mStatisticsNumFrames(0)
{    
    mWindow.setKeyRepeatEnabled(false);
    
    mFonts.load(Fonts::Main, "Sansation.ttf");
    mTextures.load(Textures::TitleScreen, "TitleScreen.png");
    
    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);
    
    registerStates();
    mStateStack.pushState(States::Title);
}

void Application::run()
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
            
            if (mStateStack.isEmpty())
                mWindow.close();
        }
        
        updateStatistics(elapsedTime);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);
        
        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}



void Application::updateStatistics(sf::Time elapsedTime)
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

void Application::render()
{
    mWindow.clear();
    
    mStateStack.draw();
    
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    
    mWindow.display();
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<PauseState>(States::Pause);
    mStateStack.registerState<LoadingState>(States::Loading);
}
