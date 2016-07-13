//
//  Player.hpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Command.hpp"
#include "KeyBinding.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <map>

class CommandQueue;

class Player : private sf::NonCopyable
{
public:
    typedef PlayerAction::Type Action;
    
    enum MissionStatus
    {
        MissionRunning,
        MissionSuccess,
        MissionFailure
    };
    
public:
    Player(sf::TcpSocket* socket, sf::Int32 identifier, const KeyBinding* binding);
    
    void handleEvent(const sf::Event& event, CommandQueue& commands);
    void handleRealtimeInput(CommandQueue& commands);
    void handleRealtimeNetworkInput(CommandQueue& commands);
    
    void handleNetworkEvent(Action action, CommandQueue& commands);
    void handleNetworkRealtimeChange(Action action, bool actionEnabled);
    
    void setMissionStatus(MissionStatus status);
    MissionStatus getMissionStatus() const;
    
    void disableAllRealtimeActions();
    bool isLocal() const;
   
private:
    void initializeActions();
    
private:
    const KeyBinding* mKeyBinding;
    std::map<Action, Command> mActionBinding;
    std::map<Action, bool> mActionProxies;
    MissionStatus mCurrentMissionStatus;
    int mIdentifier;
    sf::TcpSocket* mSocket;
    
};

#endif /* Player_hpp */
