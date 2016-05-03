//
//  TextureHolder.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 2/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "TextureHolder.hpp"
#include <cassert>

void TextureHolder::load(Textures::ID id, const std::string &filename)
{
    std::unique_ptr<sf::Texture> texture(new sf::Texture()); //Using the new keyword to store the texture into the Heap. We do this because we want the Texture to be alive as long as our game is running.
    
    if (!texture->loadFromFile(filename)) // -> is just accessing a member function/variable by a pointer.
        throw std::runtime_error("TextureHolder::load - Failed to load " + filename);
    
    auto inserted = mTextureMap.insert(std::make_pair(id, std::move(texture)));
    assert(inserted.second);
}

sf::Texture& TextureHolder::get(Textures::ID id)
{
    auto found = mTextureMap.find(id);
    assert(found != mTextureMap.end());
    
    //Here we are dereferencing the pointer with *, so we get the the actual texture value. However, because our function returns a reference to Texture, the C++ compiler is smart
    //enough to return a reference to our Texture instead. This is done automatically.
    return *found->second;
}

const sf::Texture& TextureHolder::get(Textures::ID id) const
{
    auto found = mTextureMap.find(id);
    assert(found != mTextureMap.end());
    
    //Here we are dereferencing the pointer with *, so we get the the actual texture value. However, because our function returns a reference to Texture, the C++ compiler is smart
    //enough to return a reference to our Texture instead. This is done automatically.
    return *found->second;
}
