//
//  StateStack.hpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef StateStack_hpp
#define StateStack_hpp

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

namespace sf
{
    class Event;
    class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };
    
public:
    explicit StateStack(State::Context context);
    
    template <typename T>
    void registerState(States::ID stateID);
    
    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);
    
    void pushState(States::ID stateID);
    void popState();
    void clearStates();
    
    bool isEmpty() const;
    
private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();
    
private:
    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID stateID = States::None);
        Action action;
        States::ID stateID;
    };
    
private:
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;
    
    State::Context mContext;
    
    // std::function<State::Ptr()> means a pointer to a function that returns a State::Ptr
    // with no arguments.
    std::map<States::ID, std::function<State::Ptr()>> mFactories;
};

template <typename T>
void StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this] ()
    {
        return State::Ptr(new T(*this, mContext));
    };
}


#endif /* StateStack_hpp */

