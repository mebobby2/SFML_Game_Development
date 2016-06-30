//
//  SoundPlayer.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 30/6/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "SoundPlayer.hpp"

#include <SFML/Audio/Listener.hpp>

#include <cmath>

namespace
{
    const float ListenerZ = 300.f;
    const float Attenuation = 8.f;
    const float MinDistance2D = 200.f;
    const float MinDistance3D = std::sqrt(MinDistance2D*MinDistance2D + ListenerZ*ListenerZ);
}

SoundPlayer::SoundPlayer()
: mSoundBuffers()
, mSounds()
{
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "AlliedGunfire.wav");
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "EnemyGunfire.wav");
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "Explosion1.wav");
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "Explosion2.wav");
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "LaunchMissile.wav");
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "CollectPickup.wav");
    mSoundBuffers.load(SoundEffect::AlliedGunfire, "Button.wav");
    
    sf::Listener::setDirection(0.f, 0.f, -1.f);
}

void SoundPlayer::play(SoundEffect::ID effect)
{
    play(effect, getListenerPosition());
}

void SoundPlayer::play(SoundEffect::ID effect, sf::Vector2f position)
{
    mSounds.push_back(sf::Sound());
    sf::Sound& sound = mSounds.back();
    
    sound.setBuffer(mSoundBuffers.get(effect));
    sound.setPosition(position.x, -position.y, 0.f);
    sound.setAttenuation(Attenuation);
    sound.setMinDistance(MinDistance3D);
    
    sound.play();
}

void SoundPlayer::removeStoppedSounds()
{
    mSounds.remove_if([] (const sf::Sound& s)
                      {
                          return s.getStatus() == sf::Sound::Stopped;
                      });
}

void SoundPlayer::setListenerPosition(sf::Vector2f position)
{
    sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f SoundPlayer::getListenerPosition() const
{
    sf::Vector3f position = sf::Listener::getPosition();
    return sf::Vector2f(position.x, -position.y);
}

