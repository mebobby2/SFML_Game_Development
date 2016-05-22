//
//  TextNode.hpp
//  SpaceWars
//
//  Created by Erica Ko on 22/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef TextNode_hpp
#define TextNode_hpp

#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "SceneNode.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode
{
public:
    explicit TextNode(const FontHolder& fonts, const std::string& text);
    
    void setString(const std::string& text);
    
private:
    virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
    
private:
    sf::Text mText;
};

#endif /* TextNode_hpp */

