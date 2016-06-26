//
//  TextureHolder.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 2/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "ResourceHolder.hpp"
#include "ResourcePath.hpp"
#include <cassert>

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename)
{
    std::unique_ptr<Resource> resource(new Resource()); //Using the new keyword to store the texture into the Heap. We do this because we want the Texture to be alive as long as our game is running.
    
    if (!resource->loadFromFile(resourcePath() + filename)) // -> is just accessing a member function/variable by a pointer.
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
    
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier id, const std::string &filename, const Parameter& secondParam)
{
    std::unique_ptr<Resource> resource(new Resource());
    
    if (!resource->loadFromFile(resourcePath() + filename, secondParam))
        throw std::runtime_error("ResourceHolder::load - Failed to load " + filename);
    
    insertResource(id, std::move(resource));
}

template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier id)
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    
    //Here we are dereferencing the pointer with *, so we get the the actual texture value. However, because our function returns a reference to Texture, the C++ compiler is smart
    //enough to return a reference to our Texture instead. This is done automatically.
    return *found->second;
}

template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier id) const
{
    auto found = mResourceMap.find(id);
    assert(found != mResourceMap.end());
    
    //Here we are dereferencing the pointer with *, so we get the the actual texture value. However, because our function returns a reference to Texture, the C++ compiler is smart
    //enough to return a reference to our Texture instead. This is done automatically.
    return *found->second;
}

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier id, std::unique_ptr<Resource> resource)
{
    auto inserted = mResourceMap.insert(std::make_pair(id, std::move(resource)));
    assert(inserted.second);
}

