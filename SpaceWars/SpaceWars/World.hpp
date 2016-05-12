//
//  World.hpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"
#include "SpriteNode.hpp"
#include "Aircraft.hpp"
#include "CommandQueue.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>

namespace sf {
    class RenderWindow;
}

class World : private sf::NonCopyable
{
public:
    explicit World(sf::RenderWindow& window);
    void update(sf::Time dt);
    void draw();
    CommandQueue& getCommandQueue();
    
private:
    void loadTextures();
    void buildScene();
    
private:
    enum Layer
    {
        Background,
        Air,
        LayerCount
    };
    
private:
    sf::RenderWindow& mWindow;
    sf::View mWorldView;
    TextureHolder mTextures;
    
    SceneNode mSceneGraph;
    std::array<SceneNode*, LayerCount> mSceneLayers;
    CommandQueue mCommandQueue;
    
    sf::FloatRect mWorldBounds;
    sf::Vector2f mSpawnPosition;
    float mScrollSpeed;
    Aircraft* mPlayerAircraft;
};

#endif /* World_hpp */

