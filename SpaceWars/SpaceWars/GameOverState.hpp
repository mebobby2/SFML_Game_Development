//
//  GameOverState.hpp
//  SpaceWars
//
//  Created by Erica Ko on 1/06/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef GameOverState_hpp
#define GameOverState_hpp

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class GameOverState : public State
{
public:
    GameOverState(StateStack& stack, Context context);
    
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    
private:
    sf::Text mGameOverText;
    sf::Time mElapsedTime;
};

#endif /* GameOverState_hpp */

