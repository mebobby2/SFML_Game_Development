//
//  NetworkProtocol.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 2/7/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef NetworkProtocol_hpp
#define NetworkProtocol_hpp

#include <SFML/Config.hpp>
#include <SFML/System/Vector2.hpp>

const unsigned short ServerPort = 5000;

namespace Server
{
    enum PacketType
    {
        BroadcastMessage,
        SpawnSelf,
        InitialState,
        PlayerEvent,
        PlayerRealtimeChange,
        PlayerConnect,
        PlayerDisconnect,
        AcceptCoopPartner,
        SpawnEnemy,
        SpawnPickup,
        UpdateClientState,
        MissionSuccess
    };
}

namespace Client
{
    enum PacketType
    {
        PlayerEvent,
        PlayerRealtimeChange,
        RequestCoopPartner,
        PositionUpdate,
        GameEvent,
        Quit
    };
}

namespace PlayerActions
{
    enum Action
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Fire,
        LaunchMissile,
        ActionCount
    };
}

namespace GameActions
{
    enum Type
    {
        EnemyExplode,
    };
    
    struct Action
    {
        Action()
        {
        }
        
        Action(Type type, sf::Vector2f position)
        : type(type)
        , position(position)
        {
        }
        
        Type type;
        sf::Vector2f position;
    };
}

#endif /* NetworkProtocol_hpp */

