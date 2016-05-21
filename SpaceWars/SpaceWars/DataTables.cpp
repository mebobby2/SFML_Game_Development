//
//  DataTables.cpp
//  SpaceWars
//
//  Created by Erica Ko on 21/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "DataTables.hpp"
#include "Aircraft.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"

using namespace std::placeholders;

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);
    
    data[Aircraft::Eagle].hitpoints = 100;
    data[Aircraft::Eagle].speed = 200.f;
    data[Aircraft::Eagle].fireInterval = sf::seconds(1);
    data[Aircraft::Eagle].texture = Textures::Eagle;
    
    data[Aircraft::Rapter].hitpoints = 20;
    data[Aircraft::Rapter].speed = 80.f;
    data[Aircraft::Rapter].texture = Textures::Rapter;
    data[Aircraft::Rapter].directions.push_back(Direction(+45.f, 80.f));
    data[Aircraft::Rapter].directions.push_back(Direction(-45.f, 160.f));
    data[Aircraft::Rapter].directions.push_back(Direction(+45.f, 80.f));
    data[Aircraft::Rapter].fireInterval = sf::Time::Zero;
    
    data[Aircraft::Avenger].hitpoints = 40;
    data[Aircraft::Avenger].speed = 50.f;
    data[Aircraft::Avenger].texture = Textures::Avenger;
    data[Aircraft::Avenger].directions.push_back(Direction(+45.f, 50.f));
    data[Aircraft::Avenger].directions.push_back(Direction(0.f, 50.f));
    data[Aircraft::Avenger].directions.push_back(Direction(-45.f, 100.f));
    data[Aircraft::Avenger].directions.push_back(Direction(0.f, 50.f));
    data[Aircraft::Avenger].directions.push_back(Direction(+45.f, 50.f));
    data[Aircraft::Eagle].fireInterval = sf::seconds(2);

    return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
    std::vector<ProjectileData> data(Projectile::TypeCount);
    
    data[Projectile::AlliedBullet].damage = 10;
    data[Projectile::AlliedBullet].speed = 300.f;
    data[Projectile::AlliedBullet].texture = Textures::Bullet;
    
    data[Projectile::EnemyBullet].damage = 10;
    data[Projectile::EnemyBullet].speed = 300.f;
    data[Projectile::EnemyBullet].texture = Textures::Bullet;
    
    data[Projectile::Missile].damage = 200;
    data[Projectile::Missile].speed = 150.f;
    data[Projectile::Missile].texture = Textures::Missile;
    
    return data;
}

std::vector<PickupData> initializePickupData()
{
    std::vector<PickupData> data(Pickup::TypeCount);
    
    data[Pickup::HealthRefill].texture = Textures::HealthRefill;
    data[Pickup::HealthRefill].action = std::bind(&Aircraft::repair, _1, 25);
    
    data[Pickup::MissileRefill].texture = Textures::MissileRefill;
    data[Pickup::MissileRefill].action = std::bind(&Aircraft::collectMissiles, _1, 3);
    
    data[Pickup::FireSpread].texture = Textures::FireSpread;
    data[Pickup::FireSpread].action = std::bind(&Aircraft::increaseSpread, _1);
    
    data[Pickup::FireRate].texture = Textures::FireRate;
    data[Pickup::FireRate].action = std::bind(&Aircraft::increaseFireRate, _1);
    
    return data;
}
