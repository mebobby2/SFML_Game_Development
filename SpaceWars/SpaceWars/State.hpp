//
//  State.hpp
//  SpaceWars
//
//  Created by Erica Ko on 13/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include "StateIdentifiers.hpp"
#include "ResourceIdentifiers.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf
{
    class RenderWindow;
}

class StateStack;

// These are know as forward declarations. When using forward declarations, you can only store
// references to them. That is, inside the struct Context, notice we are not storing a value of player,
// but storing a reference to it. Player* player is legal, but Player player is illegal, when using
//forward declarations.
class MusicPlayer;
class SoundPlayer;
class KeyBinding;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;
    
    struct Context
    {
        Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, MusicPlayer& music, SoundPlayer& sounds, KeyBinding& keys1, KeyBinding& keys2);
        sf::RenderWindow* window;
        TextureHolder* textures;
        FontHolder* fonts;
        MusicPlayer* music;
        SoundPlayer* sounds;
        KeyBinding* keys1;
        KeyBinding* keys2;
    };
    
public:
    State(StateStack& stack, Context context);
    virtual ~State();
    
    // The = 0 means the draw functon is a PURE virtual function. A class that has at least one pure virtual function
    // means the class itself is an abstract class and cannot be instantiated. Any subclass of State must implement
    // draw, otherwise the subclass is also considered abstract.
    virtual void draw() = 0;
    virtual bool update(sf::Time dt) = 0;
    virtual bool handleEvent(const sf::Event& event) = 0;
    
    virtual void onActivate();
    virtual void onDestroy();
    
protected:
    void requestStackPush(States::ID stateID);
    void requestStackPop();
    void requestStateClear();
    
    Context getContext() const;
    
private:
    StateStack* mStack;
    Context mContext;
};

#endif /* State_hpp */

