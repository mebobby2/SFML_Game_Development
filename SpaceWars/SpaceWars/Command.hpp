//
//  Command.hpp
//  SpaceWars
//
//  Created by Erica Ko on 11/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Command_hpp
#define Command_hpp

#include "Category.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
    Command();
    
    std::function<void(SceneNode&, sf::Time)> action;
    unsigned int category; //Category can only ever be positive numbers
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
    return [=] (SceneNode& node, sf::Time dt)
    {
        assert(dynamic_cast<GameObject*>(&node) != nullptr); //dynamic_cast is done at run-time
        
        fn(static_cast<GameObject&>(node), dt); //static_cast is done at compile time
    };
}

#endif /* Command_hpp */
