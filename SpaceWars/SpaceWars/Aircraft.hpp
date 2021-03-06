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
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "Projectile.hpp"
#include "TextNode.hpp"
#include "Animation.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Aircraft: public Entity
{
public:
    enum Type
    {
        Eagle,
        Raptor,
        Avenger,
        TypeCount
    };
    
public:
    Aircraft(Type type, const TextureHolder& textures, const FontHolder& fonts);
    
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual unsigned int getCategory() const;
    virtual sf::FloatRect getBoundingRect() const;
    virtual void remove();
    virtual bool isMarkedForRemoval() const;
    bool isAllied() const;
    float getMaxSpeed() const;
    void disablePickups();
    
    void increaseFireRate();
    void increaseSpread();
    void collectMissiles(unsigned int count);
    
    void fire();
    void launchMissile();
    void playLocalSound(CommandQueue& commands, SoundEffect::ID effect);
    int getIdentifier();
    void setIdentifier(int identifier);
    int getMissileAmmo() const;
    void setMissileAmmo(int ammo);
    
private:
    void updateMovementPattern(sf::Time dt);
    void checkPickupDrop(CommandQueue& commands);
    void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);
    
    void createBullets(SceneNode& node, const TextureHolder& textures) const;
    void createProjectile(SceneNode& node, Projectile::Type type, float xOffset, float yOffset, const TextureHolder& textures) const;
    void createPickup(SceneNode& node, const TextureHolder& textures) const;
    
    void updateTexts();
    void updateRollAnimation();
    
    
private:
    Type mType;
    sf::Sprite mSprite;
    Animation mExplosion;
    Command mFireCommand;
    Command mMissileCommand;
    sf::Time mFireCountdown;
    bool mIsFiring;
    bool mIsLaunchingMissile;
    bool mShowExplosion;
    bool mExplosionBegan;
    bool mSpawnedPickup;
    bool mPickupEnabled;
    
    int mFireRateLevel;
    int mSpreadLevel;
    int mMissileAmmo;
    
    Command mDropPickupCommand;
    float mTravelledDistance;
    std::size_t mDirectionIndex;
    TextNode* mHealthDisplay;
    TextNode* mMissileDisplay;
    
    int mIdentifier;
};


#endif /* defined(__SpaceWars__Aircraft__) */


