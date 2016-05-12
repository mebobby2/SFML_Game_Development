//
//  Player.hpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Command.hpp"
#include <SFML/Window/Event.hpp>
#include <map>

class CommandQueue;

class Player
{
public:
    enum Action
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        ActionCount
    };
    
public:
    Player();
    
    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleRealtimeInput(CommandQueue& commands);
    
    void assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getAssignedKey(Action action) const;
   
private:
    void initializeActions();
    static bool isRealtimeAction(Action action);
    
private:
    std::map<sf::Keyboard::Key, Action> mKeyBinding;
    std::map<Action, Command> mActionBinding;
    
};

#endif /* Player_hpp */
