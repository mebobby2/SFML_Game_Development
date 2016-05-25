//
//  Projectile.cpp
//  SpaceWars
//
//  Created by Erica Ko on 21/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "Projectile.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

namespace
{
    const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(Type type, const TextureHolder& textures)
: Entity(1)
, mType(type)
, mSprite(textures.get(Table[type].texture))
, mTargetDirection()
{
    centerOrigin(mSprite);
}

void Projectile::guideTowards(sf::Vector2f position)
{
    assert(isGuided());
    mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
    return mType == Missile;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if (isGuided()) {
        const float approachRate = 200.f;
        
        // mTargetDirection is a unit vector pointing towards the target.
        // Think of approachRate as the missile's speed from the perspective of the target. It's a constant.
        // We the velocity (rv) of the missile by scaling (multiplying) the approachRate by the mTargetDirection vector.
        // We then add rv to the previous velocity vector. This gives the effect that the missile is 'curving' towards
        // the target. If we do not add the previous velocity, the missile will travel in a straight line towards
        // the target instead.
        // the angle of the missile is calculated using plan on pythagaras theorem. angle = atan(o/a)
        
        sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
        newVelocity *= getMaxSpeed();
        float angle = std::atan2(newVelocity.y, newVelocity.x);
        
        setRotation(toDegree(angle) + 90.f); //Add 90 because missile should point up, instead of right
        setVelocity(newVelocity);
    }
    
    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

unsigned int Projectile::getCategory() const
{
    if (mType == EnemyBullet)
        return Category::EnemyProjectile;
    else
        return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return Table[mType].speed;
}

int Projectile::getDamage() const
{
    return Table[mType].damage;
}
