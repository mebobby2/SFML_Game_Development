//
//  TextNode.cpp
//  SpaceWars
//
//  Created by Erica Ko on 22/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "TextNode.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
    mText.setFont(fonts.get(Fonts::Main));
    mText.setCharacterSize(20);
    setString(text);
}

void TextNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(mText, states);
}

void TextNode::setString(const std::string &text)
{
    mText.setString(text);
    centerOrigin(mText);
}
