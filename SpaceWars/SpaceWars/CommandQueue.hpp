//
//  CommandQueue.hpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef CommandQueue_hpp
#define CommandQueue_hpp

#include <queue>
#include "Command.hpp"

class CommandQueue
{
public:
    void push(const Command& command);
    Command pop();
    bool isEmpty() const;
    
private:
    std::queue<Command> mQueue;
};

#endif /* CommandQueue_hpp */
