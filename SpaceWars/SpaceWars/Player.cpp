//
//  Player.cpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Aircraft.hpp"

#include <map>
#include <string>
#include <algorithm>

struct AircraftMover
{
    AircraftMover(float vx, float vy)
    : velocity(vx, vy)
    {
    }
    
    // We are overriding the '()' operator.
    // Another example, to override the '==' operator, we would use:
    //      void operator== (otherObj) 
    void operator() (Aircraft& aircraft, sf::Time) const
    {
        aircraft.accelerate(velocity);
    }
    
    sf::Vector2f velocity;
};

Player::Player()
{
    mKeyBinding[sf::Keyboard::Left] = MoveLeft;
    mKeyBinding[sf::Keyboard::Right] = MoveRight;
    mKeyBinding[sf::Keyboard::Up] = MoveUp;
    mKeyBinding[sf::Keyboard::Down] = MoveDown;
    
    initializeActions();
    
    for (auto& pair : mActionBinding)
        pair.second.category = Category::PlayerAircraft;
}

void Player::handleEvent(const sf::Event &event, CommandQueue &commands)
{
    if (event.type == sf::Event::KeyPressed) {
        auto found = mKeyBinding.find(event.key.code);
        if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
            commands.push(mActionBinding[found->second]);
    }
}

void Player::handleRealtimeInput(CommandQueue &commands)
{
    for (auto pair : mKeyBinding) {
        if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
            commands.push(mActionBinding[pair.second]);
    }
}

void Player::assignKey(Player::Action action, sf::Keyboard::Key key)
{
    for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); ) {
        if (itr->second == action)
            mKeyBinding.erase(itr++);
        else
            ++itr;
    }
    
    mKeyBinding[key] = action;
}

sf::Keyboard::Key Player::getAssignedKey(Player::Action action) const
{
    for (auto pair : mKeyBinding) {
        if (pair.second == action)
            return pair.first;
    }
    return sf::Keyboard::Unknown;
}

void Player::initializeActions()
{
    // derivedAction returns a function. The returned function itself returns void, and takes two arguments.
    // The first argument is a SceneNode&, and the second argument is sf::Time.
    // The function body then casts SceneNode to Aircraft, and calls the () method of the AircraftMover instance
    // passing in the cast Aircraft to it.
    // This enables the signiture of AircraftMover() to be 'Aircraft& aircraft, sf::Time' instead of
    // 'SceneNode& aircraft, sf::Time'. This means the code inside AircraftMover() body does not need to cast
    // SceneNode& to Aircraft& since derivedAction already does it for us.
    
    // Aircraft(playerSpeed, 0.f) instantiates a new instance of AircraftMover. Inside the derivedAction, it returns
    // a function that takes a SceneNode as parameter and calls the the AircraftMover instance's ()
    // method, ie fn(aircraftInstance, dt), passing in the SceneNode as a parameter to it.
    const float playerSpeed = 200.f;
    mActionBinding[MoveLeft].action = derivedAction<Aircraft>(AircraftMover(-playerSpeed, 0.f));
    mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+playerSpeed, 0.f));
    mActionBinding[MoveUp].action = derivedAction<Aircraft>(AircraftMover(0.f, -playerSpeed));
    mActionBinding[MoveDown].action = derivedAction<Aircraft>(AircraftMover(0.f, +playerSpeed));
}

bool Player::isRealtimeAction(Player::Action action)
{
    switch (action) {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
            return true;
            
        default:
            return false;;
    }
}
