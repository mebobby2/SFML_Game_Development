//
//  PauseState.hpp
//  SpaceWars
//
//  Created by Erica Ko on 18/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef PauseState_hpp
#define PauseState_hpp

#include "State.hpp"
#include "Container.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class PauseState : public State
{
public:
    PauseState(StateStack& stack, Context context, bool letUpdatesThrough = true);
    ~PauseState();
    
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    
private:
    sf::Sprite mBackgroundSprite;
    sf::Text mPausedText;
    GUI::Container mGUIContainer;
    bool mLetUpdatesThrough;
};

#endif /* PauseState_hpp */
