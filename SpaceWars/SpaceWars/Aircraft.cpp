//
//  Aircraft.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

Textures::ID toTextureId(Aircraft::Type type)
{
    switch (type) {
        case Aircraft::Eagle:
            return Textures::Eagle;
        case Aircraft::Rapter:
            return Textures::Rapter;
    }
}

Aircraft::Aircraft(Type type, const TextureHolder& textures)
: mType(type)
, mSprite(textures.get(toTextureId(type)))
{
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Aircraft::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}