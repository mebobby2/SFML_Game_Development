//
//  StringHelpers.cpp
//  SpaceWars
//
//  Created by Erica Ko on 10/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}
