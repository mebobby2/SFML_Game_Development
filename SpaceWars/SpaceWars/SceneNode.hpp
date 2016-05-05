//
//  SceneNode.h
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__SceneNode__
#define __SpaceWars__SceneNode__

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <memory>

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
    
    public:
        SceneNode();
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);
        void update(sf::Time dt);
        sf::Vector2f getWorldPosition() const;
        sf::Transform getWorldTransform() const;
    
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void updateCurrent(sf::Time dt);
        void updateChildren(sf::Time dt);
    
    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
};

#endif /* defined(__SpaceWars__SceneNode__) */
