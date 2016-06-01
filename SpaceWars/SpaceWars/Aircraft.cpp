//
//  Aircraft.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "Aircraft.hpp"
#include "DataTables.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "ResourceHolder.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>

namespace
{
    const std::vector<AircraftData> Table = initializeAircraftData();
}

Aircraft::Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts)
: Entity(Table[type].hitpoints)
, mType(type)
, mFireCommand()
, mMissileCommand()
, mFireCountdown(sf::Time::Zero)
, mIsFiring(false)
, mIsLaunchingMissile(false)
, mIsMarkedForRemoval(false)
, mSprite(textures.get(Table[type].texture))
, mFireRateLevel(1)
, mSpreadLevel(1)
, mMissileAmmo(2)
, mDropPickupCommand()
, mTravelledDistance(0.f)
, mDirectionIndex(0)
, mMissileDisplay(nullptr)
{
    centerOrigin(mSprite);
    
    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createBullets(node, textures);
    };
    
    mMissileCommand.category = Category::SceneAirLayer;
    mMissileCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createProjectile(node, Projectile::Missile, 0.f, 0.5f, textures);
    };
    
    mDropPickupCommand.category = Category::SceneAirLayer;
    mDropPickupCommand.action = [this, &textures] (SceneNode& node, sf::Time)
    {
        createPickup(node, textures);
    };
    
    std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
    mHealthDisplay= healthDisplay.get();
    attachChild(std::move(healthDisplay));
    
    if (getCategory() == Category::PlayerAircraft)
    {
        std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
        missileDisplay->setPosition(0, 70);
        mMissileDisplay = missileDisplay.get();
        attachChild(std::move(missileDisplay));
    }
    
    updateTexts();
}

void Aircraft::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if (isDestroyed())
    {
        checkPickupDrop(commands);
        
        mIsMarkedForRemoval = true;
        return;
    }
    
    checkProjectileLaunch(dt, commands);
    
    updateMovementPattern(dt);
    Entity::updateCurrent(dt, commands);
    
    updateTexts();
}

unsigned int Aircraft::getCategory() const
{
    if (isAllied())
        return Category::PlayerAircraft;
    else
        return Category::EnemyAircraft;
}

sf::FloatRect Aircraft::getBoundingRect() const
{
    return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Aircraft::isMarkedForRemoval() const
{
    return mIsMarkedForRemoval;
}

bool Aircraft::isAllied() const
{
    return mType == Eagle;
}

float Aircraft::getMaxSpeed() const
{
    return Table[mType].speed;
}

void Aircraft::increaseFireRate()
{
    if (mFireRateLevel < 10)
        ++mFireRateLevel;
}

void Aircraft::increaseSpread()
{
    if (mSpreadLevel < 3)
        ++mSpreadLevel;
}

void Aircraft::collectMissiles(unsigned int count)
{
    mMissileAmmo += count;
}

void Aircraft::fire()
{
    if (Table[mType].fireInterval != sf::Time::Zero)
        mIsFiring = true;
}

void Aircraft::launchMissile()
{
    if (mMissileAmmo > 0)
    {
        mIsLaunchingMissile = true;
        --mMissileAmmo;
    }
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
    const std::vector<Direction>& directions = Table[mType].directions;
    if (!directions.empty()) {
        if (mTravelledDistance > directions[mDirectionIndex].distance)
        {
            mDirectionIndex = (mDirectionIndex + 1) % directions.size();
            mTravelledDistance = 0.f;
        }
        
        // The default, 0 degree points to the right in mathematics. We do not want out enemy planes to
        // fly to the right, but we want them to fly downwards. Hence, to make the planes
        // fly downwards, we need to add 90degrees to the original angle.
        
        // Using trigonometry: the max speed (S) is the Hypotenuse, vx is the Adjacent,
        // and xy is the Opposite. We know the angle (A) already to be directions[mDirectionIndex].angle.
        // The diagram below depicts this.
        //
        //                |\
        //            vx  | \  S
        //                |A \
        //                |   \
        //                |____\
        //
        //                  vy
        //
        // To find vx, we use formula: vx = S * cos(A)
        // vy = S * sin(A).
        //
        // Representations:
        // S  = the actual speed the plane is travelling in
        // vx = the x component of the plane's actual speed
        // vy = the y component of the plane's actual speed
        //
        // Once we have vx and vy, just set these to the plane's velocity vector, which then represents
        // the plane's direction and speed combined.
        
        float radians = toRadian(directions[mDirectionIndex].angle + 90.f);
        float vx = getMaxSpeed() * std::cos(radians);
        float vy = getMaxSpeed() * std::sin(radians);
        
        setVelocity(vx, vy);
        
        // Physics formual: distance = speed * time
        mTravelledDistance += getMaxSpeed() * dt.asSeconds();
    }
}

void Aircraft::checkPickupDrop(CommandQueue &commands)
{
    if (!isAllied() && randomInt(3) == 0)
        commands.push(mDropPickupCommand);
}

void Aircraft::checkProjectileLaunch(sf::Time dt, CommandQueue &commands)
{
    if (!isAllied())
        fire();
    
    if (mIsFiring && mFireCountdown <= sf::Time::Zero)
    {
        commands.push(mFireCommand);
        mFireCountdown += Table[mType].fireInterval / (mFireRateLevel + 1.f);
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero)
    {
        mFireCountdown -= dt;
        mIsFiring = false;
    }
    
    if (mIsLaunchingMissile)
    {
        commands.push(mMissileCommand);
        mIsLaunchingMissile = false;
    }
}

void Aircraft::createBullets(SceneNode &node, const TextureHolder &textures) const
{
    Projectile::Type type = isAllied() ? Projectile::AlliedBullet : Projectile::EnemyBullet;
    
    switch (mSpreadLevel)
    {
        case 1:
            createProjectile(node, type, 0.0f, 0.5f, textures);
            break;
            
        case 2:
            createProjectile(node, type, -0.33f, 0.33f, textures);
            createProjectile(node, type, +0.33f, 0.33f, textures);
            break;
            
        case 3:
            createProjectile(node, type, -0.5f, 0.33f, textures);
            createProjectile(node, type, 0.0f, 0.5f, textures);
            createProjectile(node, type, +0.5f, 0.33f, textures);
            break;
    }
}

void Aircraft::createProjectile(SceneNode &node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder &textures) const
{
    std::unique_ptr<Projectile> projectile(new Projectile(type, textures));
    
    sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
    sf::Vector2f velocity(0, projectile->getMaxSpeed());
    
    float sign = isAllied() ? -1.f : +1.f;
    projectile->setPosition(getWorldPosition() + offset * sign);
    projectile->setVelocity(velocity * sign);
    node.attachChild(std::move(projectile));
}

void Aircraft::createPickup(SceneNode &node, const TextureHolder &textures) const
{
    auto type = static_cast<Pickup::Type>(randomInt(Pickup::TypeCount));
    
    std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
    pickup->setPosition(getWorldPosition());
    pickup->setVelocity(0.f, 1.f);
    node.attachChild(std::move(pickup));
}

void Aircraft::updateTexts()
{
    mHealthDisplay->setString(toString(getHitpoints()) + " HP");
    mHealthDisplay->setPosition(0.f, 50.f);
    mHealthDisplay->setRotation(-getRotation());
    
    if (mMissileDisplay) {
        if (mMissileAmmo == 0)
            mMissileDisplay->setString("");
        else
            mMissileDisplay->setString("M: " + toString(mMissileAmmo));
    }
}
