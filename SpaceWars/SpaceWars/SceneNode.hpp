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

#include "Category.hpp"

#include <vector>
#include <memory>
#include <set>

struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, private sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
        typedef std::pair<SceneNode*, SceneNode*> Pair;
    
    public:
        explicit SceneNode(Category::Type category = Category::None);
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);
        void update(sf::Time dt, CommandQueue& commands);
        sf::Vector2f getWorldPosition() const;
        sf::Transform getWorldTransform() const;
        void removeWrecks();
        void checkSceneCollisions(SceneNode& sceneGraph, std::set<Pair>& collisionPairs);
        virtual sf::FloatRect getBoundingRect() const;
    
        void onCommand(const Command& command, sf::Time dt);
        virtual unsigned int getCategory() const;
    
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
        void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
    
        virtual void updateCurrent(sf::Time dt);
        void updateChildren(sf::Time dt);
    
    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
};

float distance(const SceneNode& lhs, const SceneNode& rhs);

#endif /* defined(__SpaceWars__SceneNode__) */
