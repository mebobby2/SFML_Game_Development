//
//  KeyBinding.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 11/7/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef KeyBinding_hpp
#define KeyBinding_hpp

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>

namespace PlayerAction
{
    enum Type
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Fire,
        LaunchMissile,
        Count
    };
}

class KeyBinding
{
public:
    typedef PlayerAction::Type Action;
    
public:
    explicit KeyBinding(int controlPrefconfiguration);
    
    void assignKey(Action action, sf::Keyboard::Key key);
    sf::Keyboard::Key getAssignedKey(Action action) const;
    
    bool checkAction(sf::Keyboard::Key key, Action& out) const;
    std::vector<Action> getRealtimeActions() const;
    
private:
    void initializeActions();
    
private:
    std::map<sf::Keyboard::Key, Action> mKeyMap;
};

bool isRealtimeAction(PlayerAction::Type action);

#endif /* KeyBinding_hpp */
