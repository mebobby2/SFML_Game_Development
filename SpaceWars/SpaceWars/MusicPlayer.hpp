//
//  MusicPlayer.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 30/6/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef MusicPlayer_hpp
#define MusicPlayer_hpp

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Audio/Music.hpp>

#include <map>
#include <string>

class MusicPlayer : private sf::NonCopyable
{
public:
    MusicPlayer();
    
    void play(Music::ID theme);
    void stop();
    
    void setPaused(bool paused);
    void setVolume(float volume);
    
private:
    sf::Music mMusic;
    std::map<Music::ID, std::string> mFilenames;
    float mVolume;
};

#endif /* MusicPlayer_hpp */



