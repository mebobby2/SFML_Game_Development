//
//  StateStack.cpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "StateStack.hpp"

#include <cassert>

StateStack::StateStack(State::Context context)
: mStack()
, mPendingList()
, mContext(context)
, mFactories()
{
}

void StateStack::update(sf::Time dt)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->update(dt))
            break;
    }
    
    applyPendingChanges();
}


void StateStack::draw()
{
    for (State::Ptr& state : mStack)
        state->draw();
}

void StateStack::handleEvent(const sf::Event &event)
{
    for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
    {
        if (!(*itr)->handleEvent(event))
            break;
    }
    
    applyPendingChanges();
}


void StateStack::pushState(States::ID stateID)
{
    // MyClass *object = new MyClass(); //object is on the heap,  calls MyClass::MyClass()
    // MyClass object; //object is on the stack, calls Myclass::MyClass()
    //
    // MyClass object(10) //calls MyClass::MyClass(int)
    
    // Notice we are instantiating the PendingChange object onto the stack and NOT the heap since
    // we are not using the 'new' keyword. You know that objects created on the stack are deallocated
    // when the function body ends. Hence, the PendingChange object we just instantiated will be
    // lost when the function ends. However, before the function ends, we pass the PendingChange object
    // to the push_back method of the vector, which COPIES the passed in object to the end of the vector.
    // This is why we can instantiate the PendingChange object on the stack since its temporary because the
    // push_back method will make a copy of it anyways.
    mPendingList.push_back(PendingChange(Push, stateID));
}


void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty() const
{
    return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());
    
    return found->second();
}


void StateStack::applyPendingChanges()
{
    for (PendingChange change : mPendingList) {
        switch (change.action) {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;
                
            case Pop:
                mStack.pop_back();
                break;
                
            case Clear:
                mStack.clear();
                break;
        }
    }
    
    mPendingList.clear();
}


StateStack::PendingChange::PendingChange(Action action, States::ID stateID)
: action(action)
, stateID(stateID)
{
}