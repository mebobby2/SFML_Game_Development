//
//  SpriteNode.hpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef SpriteNode_hpp
#define SpriteNode_hpp


#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

class SpriteNode: public SceneNode
{
public:
    explicit    SpriteNode(const sf::Texture& texture);
    SpriteNode(const sf::Texture& texture, const sf::IntRect& textureReact);
    
private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    
private:
    sf::Sprite mSprite;
};


#endif /* SpriteNode_hpp */


