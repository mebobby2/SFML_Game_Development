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
#include <SFML/Graphics/Sprite.hpp>
#include "ResourceIdentifiers.hpp"

class Aircraft: public Entity
{
    public:
        enum Type
        {
            Eagle,
            Rapter,
        };
    
    public:
            Aircraft(Type type, const TextureHolder& textures);
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual unsigned int getCategory() const;
    
    private:
        Type mType;
        sf::Sprite mSprite;
};


#endif /* defined(__SpaceWars__Aircraft__) */
