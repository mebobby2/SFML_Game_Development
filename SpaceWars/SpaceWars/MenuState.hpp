//
//  MenuState.hpp
//  SpaceWars
//
//  Created by Erica Ko on 18/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef MenuState_hpp
#define MenuState_hpp

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class MenuState : public State
{
public:
    MenuState(StateStack& stack, Context context);
    
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    
private:
    sf::Sprite mBackgroundSprite;
    GUI::Container mGUIContainer;
};

#endif /* MenuState_hpp */

