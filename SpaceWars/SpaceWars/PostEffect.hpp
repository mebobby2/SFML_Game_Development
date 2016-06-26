//
//  PostEffect.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 26/6/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef PostEffect_hpp
#define PostEffect_hpp

#include <SFML/System/NonCopyable.hpp>

namespace sf
{
    class RenderTarget;
    class RenderTexture;
    class Shader;
}

class PostEffect : sf::NonCopyable
{
public:
    virtual ~PostEffect();
    virtual void apply(const sf::RenderTexture& input, sf::RenderTarget& output) = 0;
    
    static bool isSupported();
    
protected:
    static void applyShader(const sf::Shader& shader, sf::RenderTarget& output);
};

#endif /* PostEffect_hpp */

