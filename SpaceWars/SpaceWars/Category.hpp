//
//  Category.hpp
//  SpaceWars
//
//  Created by Erica Ko on 11/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Category_hpp
#define Category_hpp

namespace Category
{
    // The integer 1 in binary is 0000 0001
    // << is the left shift operator
    // Therefore, 1 << 1 = 0000 0010
    // And 1 << 2 = 0000 0100
    enum Type
    {
        None             = 0,
        SceneAirLayer    = 1 << 0,
        PlayerAircraft   = 1 << 1,
        AlliedAircraft   = 1 << 2,
        EnemyAircraft    = 1 << 3,
        Pickup           = 1 << 4,
        AlliedProjectile = 1 << 5,
        EnemyProjectile  = 1 << 6,
        ParticleSystem   = 1 << 7,
        
        Aircraft         = PlayerAircraft | AlliedAircraft | EnemyAircraft,
        Projectile       = AlliedProjectile | EnemyProjectile,
    };
}

#endif /* Category_hpp */



