//
//  Game.h
//  SpaceWars
//
//  Created by Bobby Lei on 29/04/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__Game__
#define __SpaceWars__Game__

#include <SFML/System/Time.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "World.hpp"

class Game : private sf::NonCopyable
{
public:
    Game();
    void run();
    
private:
    void processEvents();
    void update(sf::Time elapsedTime);
    void render();
    
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    
private:
    static const sf::Time TimePerFrame;
    
    sf::RenderWindow mWindow;
    World mWorld;
};

#endif /* defined(__SpaceWars__Game__) */


