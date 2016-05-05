//
//  Aircraft.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "Aircraft.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Aircraft::Aircraft(Type type): mType(type)
{
}

void Aircraft::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}