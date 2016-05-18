//
//  TitleState.hpp
//  SpaceWars
//
//  Created by Erica Ko on 18/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef TitleState_hpp
#define TitleState_hpp

#include "State.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class TitleState: public State
{
public:
    TitleState(StateStack& stack, Context context);
    
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    
private:
    sf::Sprite mBackgroundSprite;
    sf::Text mText;
    
    bool mShowText;
    sf::Time mTextEffectTime;
};

#endif /* TitleState_hpp */

