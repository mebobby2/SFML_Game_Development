//
//  DataTables.hpp
//  SpaceWars
//
//  Created by Erica Ko on 21/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef DataTables_hpp
#define DataTables_hpp

#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>
#include <functional>

class Aircraft;

struct Direction
{
    Direction(float angle, float distance)
    : angle(angle)
    , distance(distance)
    {
    }
    
    float angle;
    float distance;
};

struct AircraftData
{
    int hitpoints;
    float speed;
    Textures::ID texture;
    sf::Time fireInterval;
    std::vector<Direction> directions;
};

struct ProjectileData
{
    int damage;
    float speed;
    Textures::ID texture;
};

struct PickupData
{
    std::function<void(Aircraft&)> action;
    Textures::ID texture;
};

std::vector<AircraftData> initializeAircraftData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();

#endif /* DataTables_hpp */

