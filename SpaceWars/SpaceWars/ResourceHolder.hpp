//
//  TextureHolder.h
//  SpaceWars
//
//  Created by Bobby Lei on 2/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__TextureHolder__
#define __SpaceWars__TextureHolder__

#include <map>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

template <typename Resource, typename Identifier>
class ResourceHolder
{
    public:
        void load(Identifier id, const std::string& filename);
    
        template <typename Parameter>
        void load(Identifier id, const std::string& filename, const Parameter& secondParam);
    
    
        Resource& get(Identifier id);
        //If we only have a pointer or reference to a TextureHolder at hand, we need to provide a const-qualified overload for this function
        //to be callable. E.g. if we have:
        //  const TextureHolder textureHolder;
        //And when we call get on this object, the compiler will the const-qualified method 'const sf::Texture& get(Textures::ID id) const'
        //If we have:
        //  TextureHolder textureHolder;
        //The compiler will call the 'sf::Texture& get(Textures::ID id)' function.
        const Resource& get(Identifier id) const;
    
    private:
        void insertResource(Identifier id, std::unique_ptr<Resource> resource);
    
    private:
        std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

// ResourceHolder uses templates. When using templates, the complete implementation needs to be in the header. We cannot
// use .cpp files for the method definitions anymore, but we would still like to seperate interface and implmentation.
// That is why we use an 'inl' file for the method definitions and include them in the header file. inl is a common file
// extension for inline template implementations.
#include "ResourceHolder.inl"

#endif /* defined(__SpaceWars__TextureHolder__) */

