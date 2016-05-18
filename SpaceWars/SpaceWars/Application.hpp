//
//  Game.h
//  SpaceWars
//
//  Created by Bobby Lei on 29/04/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__Game__
#define __SpaceWars__Game__

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Player.hpp"
#include "StateStack.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
public:
    Application();
    void run();
    
private:
    void processInput();
    void update(sf::Time elapsedTime);
    void render();
    
    void updateStatistics(sf::Time elapsedTime);
    void registerStates();
    
private:
    static const sf::Time TimePerFrame;
    
    sf::RenderWindow mWindow;
    TextureHolder mTextures;
    FontHolder mFonts;
    Player mPlayer;
    
    StateStack mStateStack;
    
    sf::Text mStatisticsText;
    sf::Time mStatisticsUpdateTime;
    std::size_t mStatisticsNumFrames;
};

#endif /* defined(__SpaceWars__Game__) */