//
//  Label.hpp
//  SpaceWars
//
//  Created by Erica Ko on 20/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef Label_hpp
#define Label_hpp

#include "Component.hpp"
#include "ResourceIdentifiers.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/Text.hpp>

namespace GUI
{
    class Label : public Component
    {
    public:
        typedef std::shared_ptr<Label> Ptr;
        
    public:
        Label(const std::string& text, const FontHolder& fonts);
        
        virtual bool isSelectable() const;
        void setText(const std::string& text);
        virtual void handleEvent(const sf::Event& event);
        
    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        
    private:
        sf::Text mText;
    };
}

#endif /* Label_hpp */
