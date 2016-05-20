//
//  Pickup.hpp
//  SpaceWars
//
//  Created by Erica Ko on 21/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Pickup_hpp
#define Pickup_hpp

#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Aircraft;

class Pickup : public Entity
{
public:
    enum Type
    {
        HealthRefill,
        MissileRefill,
        FireSpread,
        FireRate,
        TypeCount
    };
    
public:
    Pickup(Type type, const TextureHolder& textures);
    
    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    
    void apply(Aircraft& player) const;
    
protected:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    
private:
    Type mType;
    sf::Sprite mSprite;
};

#endif /* Pickup_hpp */
