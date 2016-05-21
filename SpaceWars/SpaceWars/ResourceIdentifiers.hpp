//
//  ResourceIdentifiers.h
//  SpaceWars
//
//  Created by Bobby Lei on 5/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__ResourceIdentifiers__
#define __SpaceWars__ResourceIdentifiers__

namespace sf
{
    class Texture;
    class Font;
}

namespace Textures
{
    enum ID
    {
        Eagle,
        Rapter,
        Avenger,
        Bullet,
        Missile,
        Desert,
        HealthRefill,
        MissileRefill,
        FireSpread,
        FireRate,
        TitleScreen,
        ButtonNormal,
        ButtonSelected,
        ButtonPressed
    };
}

namespace Fonts
{
    enum ID
    {
        Main,
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif /* defined(__SpaceWars__ResourceIdentifiers__) */
