//
//  ParticleNode.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 25/6/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef ParticleNode_hpp
#define ParticleNode_hpp

#include "SceneNode.hpp"
#include "ResourceIdentifiers.hpp"
#include "Particle.hpp"

#include <SFML/Graphics/VertexArray.hpp>

#include <deque>

class ParticleNode : public SceneNode
{
public:
    ParticleNode(Particle::Type type, const TextureHolder& textures);
    
    void addParticles(sf::Vector2f position);
    Particle::Type getParticleType() const;
    virtual unsigned int getCategory() const;
    
private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    
    void addVertex(float worldX, float worldY, float textCoordX, float textCoordY, const sf::Color& color) const;
    void computeVertices() const;
    
private:
    std::deque<Particle> mParticles;
    const sf::Texture& mTexture;
    Particle::Type mType;
    
    mutable sf::VertexArray mVertexArray;
    mutable bool mNeedsVertexUpdate;
};

#endif /* ParticleNode_hpp */

