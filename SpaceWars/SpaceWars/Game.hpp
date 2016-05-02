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
    void    update(sf::Time deltaTime);
        void    render();
    
        void    handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
    
    private:
        static const float PlayerSpeed;
        static const sf::Time TimePerFrame;
    
        sf::RenderWindow mWindow;
        sf::Texture mTexture;
        sf::Sprite mPlayer;
    
        bool mIsMovingUp;
        bool mIsMovingDown;
        bool mIsMovingRight;
        bool mIsMovingLeft;
};

#endif /* defined(__SpaceWars__Game__) */
