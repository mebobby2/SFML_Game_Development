//
//  Entity.h
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__Entity__
#define __SpaceWars__Entity__

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "SceneNode.hpp"

class Entity : public SceneNode
{
    public:
        void setVelocity(sf::Vector2f velocity);
        void setVelocity(float vx, float vy);
        sf::Vector2f getVelocity() const;
    
    private:
        sf::Vector2f mVelocity;
        virtual void updateCurrent(sf::Time dt);
};



#endif /* defined(__SpaceWars__Entity__) */
