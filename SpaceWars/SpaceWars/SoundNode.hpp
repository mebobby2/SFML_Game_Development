//
//  SoundNode.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 2/7/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef SoundNode_hpp
#define SoundNode_hpp

#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
    explicit SoundNode(SoundPlayer& player);
    void playSound(SoundEffect::ID sound, sf::Vector2f position);
    
    virtual unsigned int getCategory() const;
    
private:
    SoundPlayer& mSounds;
};

#endif /* SoundNode_hpp */

