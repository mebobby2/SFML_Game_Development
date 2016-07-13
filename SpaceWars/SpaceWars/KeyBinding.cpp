//
//  KeyBinding.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 11/7/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "KeyBinding.hpp"

#include <string>
#include <algorithm>

KeyBinding::KeyBinding(int controlPreconfiguration)
: mKeyMap()
{
    if (controlPreconfiguration == 1)
    {
        mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveLeft;
        mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveRight;
        mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveUp;
        mKeyMap[sf::Keyboard::Left] = PlayerAction::MoveDown;
        mKeyMap[sf::Keyboard::Left] = PlayerAction::Fire;
        mKeyMap[sf::Keyboard::Left] = PlayerAction::LaunchMissile;
    }
    else if (controlPreconfiguration == 2)
    {
        mKeyMap[sf::Keyboard::A] = PlayerAction::MoveLeft;
        mKeyMap[sf::Keyboard::D] = PlayerAction::MoveRight;
        mKeyMap[sf::Keyboard::W] = PlayerAction::MoveUp;
        mKeyMap[sf::Keyboard::S] = PlayerAction::MoveDown;
        mKeyMap[sf::Keyboard::F] = PlayerAction::Fire;
        mKeyMap[sf::Keyboard::R] = PlayerAction::LaunchMissile;
    }
}

void KeyBinding::assignKey(Action action, sf::Keyboard::Key key)
{
    for (auto itr = mKeyMap.begin(); itr != mKeyMap.end();)
    {
        if (itr->second == action)
            mKeyMap.erase(itr++);
        else
            ++itr;
    }
    
    mKeyMap[key] = action;
}

sf::Keyboard::Key KeyBinding::getAssignedKey(Action action) const
{
    for (auto pair : mKeyMap)
    {
        if (pair.second == action)
            return pair.first;
    }
    
    return sf::Keyboard::Unknown;
}

bool KeyBinding::checkAction(sf::Keyboard::Key key, Action &out) const
{
    auto found = mKeyMap.find(key);
    if (found == mKeyMap.end())
    {
        return false;
    }
    else
    {
        out = found->second;
        return true;
    }
}

std::vector<KeyBinding::Action> KeyBinding::getRealtimeActions() const
{
    std::vector<Action> actions;
    
    for (auto pair : mKeyMap)
    {
        if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
            actions.push_back(pair.second);
    }
    
    return actions;
}

bool isRealtimeAction(PlayerAction::Type action)
{
    switch (action) {
        case PlayerAction::MoveLeft:
        case PlayerAction::MoveRight:
        case PlayerAction::MoveDown:
        case PlayerAction::MoveUp:
        case PlayerAction::Fire:
            return true;
            
        default:
            return false;
    }
}
