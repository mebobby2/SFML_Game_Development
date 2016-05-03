//
//  Aircraft.h
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__Aircraft__
#define __SpaceWars__Aircraft__

#include "Entity.hpp"

class Aircraft: public Entity
{
    public:
        enum Type
        {
            Eagle,
            Rapter,
        };
    
    public:
        explicit    Aircraft(Type type);
    
    private:
        Type mType;
};


#endif /* defined(__SpaceWars__Aircraft__) */
