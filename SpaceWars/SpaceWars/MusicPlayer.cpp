//
//  MusicPlayer.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 30/6/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "MusicPlayer.hpp"
#include "ResourcePath.hpp"

MusicPlayer::MusicPlayer()
: mMusic()
, mFilenames()
, mVolume(100.f)
{
    mFilenames[Music::MenuTheme] = "MenuTheme.ogg";
    mFilenames[Music::MissionTheme] = "MissionTheme.ogg";
}

void MusicPlayer::play(Music::ID theme)
{
    std::string filename = mFilenames[theme];
    
    if (!mMusic.openFromFile(resourcePath() + filename))
        throw std::runtime_error("Music " + filename + " could not be loaded.");
    
    mMusic.setVolume(mVolume);
    mMusic.setLoop(true);
    mMusic.play();
}

void MusicPlayer::stop()
{
    mMusic.stop();
}

void MusicPlayer::setVolume(float volume)
{
    mVolume = volume;
}

void MusicPlayer::setPaused(bool paused)
{
    if (paused)
        mMusic.pause();
    else
        mMusic.play();
}
