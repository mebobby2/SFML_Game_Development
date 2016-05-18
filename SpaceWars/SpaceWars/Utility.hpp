//
//  Utility.hpp
//  SpaceWars
//
//  Created by Erica Ko on 18/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Utility_hpp
#define Utility_hpp

#include <sstream>

namespace sf
{
    class Sprite;
    class Text;
}

template <typename T>
std::string toString(const T& value);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);

#include "Utility.inl"
#endif /* Utility_hpp */

