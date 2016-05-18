//
//  LoadingState.hpp
//  SpaceWars
//
//  Created by Erica Ko on 19/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef LoadingState_hpp
#define LoadingState_hpp

#include "State.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ParallelTask.hpp"

class LoadingState : public State
{
public:
    LoadingState(StateStack& stack, Context context);
    
    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event& event);
    
    void setCompletion(float percent);
    
private:
    sf::Text mLoadingText;
    sf::RectangleShape mProgressBarBackground;
    sf::RectangleShape mProgressBar;
    
    ParallelTask mLoadingTask;
};

#endif /* LoadingState_hpp */

