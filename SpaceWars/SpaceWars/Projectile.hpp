//
//  Projectile.hpp
//  SpaceWars
//
//  Created by Erica Ko on 21/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Projectile_hpp
#define Projectile_hpp

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
public:
    enum Type
    {
        AlliedBullet,
        EnemyBullet,
        Missile,
        TypeCount
    };
    
public:
    Projectile(Type type, const TextureHolder& textures);
    
    void guideTowards(sf::Vector2f position);
    bool isGuided() const;
    
    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    float getMaxSpeed() const;
    int getDamage() const;
    
private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    
private:
    Type mType;
    sf::Sprite mSprite;
    sf::Vector2f mTargetDirection;
    
};

#endif /* Projectile_hpp */

