//
//  Game.h
//  SpaceWars
//
//  Created by Bobby Lei on 29/04/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__Game__
#define __SpaceWars__Game__

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Game
{
    public:
                Game();
        void    run();
    private:
        void    processEvents();
        void    update();
        void    render();
    
    private:
        sf::RenderWindow mWindow;
        sf::CircleShape mPlayer;
};

#endif /* defined(__SpaceWars__Game__) */
