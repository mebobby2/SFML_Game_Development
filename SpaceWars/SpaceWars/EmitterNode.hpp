//
//  EmitterNode.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 25/6/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef EmitterNode_hpp
#define EmitterNode_hpp

#include "SceneNode.hpp"
#include "Particle.hpp"

class ParticleNode;

class EmitterNode: public SceneNode
{
public:
    explicit EmitterNode(Particle::Type type);
    
private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    void emitParticles(sf::Time dt);
    
private:
    sf::Time mAccumulatedTime;
    Particle::Type mType;
    
    //This is a pointer because there is only two instances of our particle system.
    //One for smoke and one for propellant. Therefore, EmitterNode is responsible for
    //finding the correct particle system to use and referencing it.
    ParticleNode* mParticleSystem;
};

#endif /* EmitterNode_hpp */

