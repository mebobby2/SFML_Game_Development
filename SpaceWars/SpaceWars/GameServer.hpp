//
//  GameServer.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 2/7/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef GameServer_hpp
#define GameServer_hpp

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Thread.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Sleep.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/TcpSocket.hpp>

#include <vector>
#include <memory>
#include <map>

class GameServer
{
public:
    explicit GameServer(sf::Vector2f battlefieldSize);
    ~GameServer();
    
    void notifyPlayerSpawn(sf::Int32 aircraftIdentifier);
    void notifyPlayerRealtimeChange(sf::Int32 aircraftIdentifier, sf::Int32 action, bool actionEnabled);
    void notifyPlayerEvent(sf::Int32 aircraftIdentifier, sf::Int32 action);
    
private:
    struct RemotePeer
    {
        RemotePeer();
        sf::TcpSocket socket;
        sf::Time lastPacketTime;
        std::vector<sf::Int32> aircraftIdentifiers;
        bool ready;
        bool timedOut;
    };
    
    struct AircraftInfo
    {
        sf::Vector2f position;
        sf::Int32 hitpoints;
        sf::Int32 missileAmmo;
        std::map<sf::Int32, bool> realtimeActions;
    };
    
    typedef std::unique_ptr<RemotePeer> PeerPtr;
    
private:
    void setListening(bool enable);
    void executionThread();
    void tick();
    sf::Time now() const;
    
    void handleIncomingPackets();
    void handleIncomingPacket(sf::Packet packet, RemotePeer& receivingPeer, bool& detectedTimeout);
    
    void handleIncomingConnections();
    void handleDisconnections();
    
    void informWorldState(sf::TcpSocket& socket);
    void broadcastMessage(const std::string& message);
    void sendToAll(sf::Packet& packet);
    void updateClientState();
    
private:
    sf::Thread mThread;
    sf::Clock mClock;
    sf::TcpListener mListenerSocket;
    bool mListeningState;
    sf::Time mClientTimeoutTime;
    
    //std::size_t is the type of any sizeof expression and as is guaranteed to be able to
    //express the maximum size of any object (including any array) in C++.
    std::size_t mMaxConnectedPlayers;
    std::size_t mConnectedPlayers;
    
    float mWorldHeight;
    sf::FloatRect mBattleFieldRect;
    float mBattleFieldScrollSpeed;
    
    std::size_t mAircraftCount;
    std::map<sf::Int32, AircraftInfo> mAircraftInfo;
    
    std::vector<PeerPtr> mPeers;
    sf::Int32 mAircraftIdentifierCounter;
    bool mWaitingThreadEnd;
    
    sf::Time mLastSpawnTime;
    sf::Time mTimeForNextSpawn;
};

#endif /* GameServer_hpp */
