//
//  Particle.hpp
//  SpaceWars
//
//  Created by Erica Ko on 9/06/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

struct Particle
{
    enum Type
    {
        Propellant,
        Smoke,
        ParticleCount
    };
    
    sf::Vector2f position;
    sf::Color color;
    sf::Time lifetime;
};

#endif /* Particle_hpp */

