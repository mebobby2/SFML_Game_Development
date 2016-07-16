//
//  GameState.hpp
//  SpaceWars
//
//  Created by Erica Ko on 18/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef GameState_hpp
#define GameState_hpp

#include "State.hpp"
#include "World.hpp"
#include "Player.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);
    
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
  
private:
    World mWorld;
    Player mPlayer;
    
};

#endif /* GameState_hpp */