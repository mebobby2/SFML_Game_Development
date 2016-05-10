//
//  StringHelpers.hpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef StringHelpers_hpp
#define StringHelpers_hpp

#include <sstream>

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string toString(const T& value);

#include "StringHelpers.inl"
#endif /* StringHelpers_hpp */
