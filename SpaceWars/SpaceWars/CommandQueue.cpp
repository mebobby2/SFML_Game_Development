//
//  CommandQueue.cpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "CommandQueue.hpp"
#include "SceneNode.hpp"

void CommandQueue::push(const Command &command)
{
    mQueue.push(command);
}

Command CommandQueue::pop()
{
    Command command = mQueue.front();
    mQueue.pop();
    return command;
}

bool CommandQueue::isEmpty() const
{
    return mQueue.empty();
}
