//
//  TextureHolder.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 2/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "TextureHolder.hpp"

void TextureHolder::load(Textures::ID id, const std::string &filename)
{
    std::unique_ptr<sf::Texture> texture(new sf::Texture()); //Using the new keyword to store the texture into the Heap. We do this because we want the Texture to be alive as long as our game is running.
    texture->loadFromFile(filename); // -> is just accessing a member function/variable by a pointer.
    
    mTextureMap.insert(std::make_pair(id, std::move(texture)));
}

sf::Texture& TextureHolder::get(Textures::ID id)
{
    auto found = mTextureMap.find(id);
    
    //Here we are dereferencing the pointer with *, so we get the the actual texture value. However, because our function returns a reference to Texture, the C++ compiler is smart
    //enough to return a reference to our Texture instead. This is done automatically.
    return *found->second;
}

const sf::Texture& TextureHolder::get(Textures::ID id) const
{
    auto found = mTextureMap.find(id);
    
    //Here we are dereferencing the pointer with *, so we get the the actual texture value. However, because our function returns a reference to Texture, the C++ compiler is smart
    //enough to return a reference to our Texture instead. This is done automatically.
    return *found->second;
}
