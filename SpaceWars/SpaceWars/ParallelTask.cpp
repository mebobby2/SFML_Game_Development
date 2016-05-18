//
//  ParallelTask.cpp
//  SpaceWars
//
//  Created by Erica Ko on 19/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "ParallelTask.hpp"

ParallelTask::ParallelTask()
: mThread(&ParallelTask::runTask, this)
, mFinished(false)
{
}

void ParallelTask::execute()
{
    mFinished = false;
    mElapsedTime.restart();
    mThread.launch();
}

bool ParallelTask::isFinished()
{
    sf::Lock lock(mMutex);
    return mFinished;
}

float ParallelTask::getCompletion()
{
    sf::Lock lock(mMutex);
    return mElapsedTime.getElapsedTime().asSeconds() / 10.f;
}

void ParallelTask::runTask()
{
    bool ended = false;
    while (!ended)
    {
        sf::Lock lock(mMutex);
        if (mElapsedTime.getElapsedTime().asSeconds() >= 10.f)
            ended = true;
    }
    
    // sf::Lock is a RAII compliant class, as soon as it goes out of scope and is
    // destructed, the sf::Mutex object automatically unlocks. Hence, the extra brackets that follow
    // are just a way of releasing that same lock as soon as possible, so the variable is then
    // available for other threads to access. In this case, it releases the lock that locked the access
    // of the mElapsedTime variable above.
    {
        sf::Lock lock(mMutex);
        mFinished = true;
    }
}
