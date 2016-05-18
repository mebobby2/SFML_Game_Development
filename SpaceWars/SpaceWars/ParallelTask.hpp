//
//  ParallelTask.hpp
//  SpaceWars
//
//  Created by Erica Ko on 19/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef ParallelTask_hpp
#define ParallelTask_hpp

#include <SFML/System/Thread.hpp>
#include <SFML/System/Mutex.hpp>
#include <SFML/System/Lock.hpp>
#include <SFML/System/Clock.hpp>

class ParallelTask
{
public:
    ParallelTask();
    void execute();
    bool isFinished();
    float getCompletion();
    
private:
    void runTask();
    
private:
    sf::Thread mThread;
    bool mFinished;
    sf::Clock mElapsedTime;
    sf::Mutex mMutex;
};

#endif /* ParallelTask_hpp */

