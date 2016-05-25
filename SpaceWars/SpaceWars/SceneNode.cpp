//
//  SceneNode.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "SceneNode.hpp"
#include "Command.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <cassert>
#include <algorithm>
#include <cmath>

SceneNode::SceneNode(Category::Type category)
: mChildren()
, mParent(nullptr)
, mDefaultCategory(category)
{
}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode &node)
{
    // .get() returns a pointer to the object pointed by the unique_ptr object.
    // A pointer stores the memory address of another variable.
    
    // References in C++ are 'aliases' for another variable (ie a reference is a variable which
    // refers to another variable), and the main reason they are added
    // to the language is to support pass by reference in function calls. That is why C++
    // makes it nice to work with references in that you do not need to explicitly dereference
    // them, using *, like you do when using pointers. It is understood that you are intending
    // to work with the referred variable when using references.
    // &node returns the memory address of the node which the reference is pointing to.
    
    // Therefore, p.get() == &node is testing whether the address stored by the pointer p.get()
    // is equal to the memory address of the node referenced by &node.
    // We need to explicitly use '&' in front of node in '&node' because C++ implicitly dereferences
    // references for us, but we do not want the value the &node is pointed to, we want the address,
    // hence we need to explicitly get the memory address with the '&' operator.
    // Since pointers are not implicitly deferences, p.get() will return the address of the node as
    // opposed to returning the valye at that address.
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                              [&] (Ptr& p) -> bool { return p.get() == &node; }); //[&] captures all automatic variables, ie 'found' by reference
    assert(found != mChildren.end());
    
    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::updateCurrent(sf::Time, CommandQueue&)
{
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    for (const Ptr& child : mChildren) {
        child->update(dt, commands);
    }
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    
    drawCurrent(target, states);
    drawChildren(target, states);
    
    // Draw bounding rectangle - disabled by default
    //drawBoundingRect(target, states);
}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const Ptr& child : mChildren) {
        child->draw(target, states);
    }
}

void SceneNode::drawCurrent(sf::RenderTarget&, sf::RenderStates) const
{
    // Do nothing by default
}

void SceneNode::drawBoundingRect(sf::RenderTarget &target, sf::RenderStates states) const
{
    sf::FloatRect rect = getBoundingRect();
    
    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);
    
    target.draw(shape);
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;
    
    for (const SceneNode* node = this; node != nullptr; node = node->mParent) {
        transform = node->getTransform() * transform;
    }
    
    return transform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

void SceneNode::onCommand(const Command &command, sf::Time dt)
{
    if (command.category & getCategory())
        command.action(*this, dt);
    
    for (const Ptr& child : mChildren)
        child->onCommand(command, dt);
}

unsigned int SceneNode::getCategory() const
{
    return mDefaultCategory;
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph, std::set<Pair> &collisionPairs)
{
    checkNodeCollision(sceneGraph, collisionPairs);
    
    // child is a pointer to a SceneNode. Before passing in child to checkSceneCollision, we dereference it.
    // But checkSceneCollision takes a reference to a SceneNode as an argument, so why are we passing in a value
    // of SceneNode instead of the reference. C++ handles this automatically for us, if we pass in a value but
    // the function takes a reference, C++ automatically converts the value to a reference and then pass it into
    // the function.
    for (Ptr& child : sceneGraph.mChildren)
        checkSceneCollision(*child, collisionPairs);
}

void SceneNode::checkNodeCollision(SceneNode &node, std::set<Pair> &collisionPairs)
{
    // this != &node. node is already a reference to a SceneNode. But we want the memory address of the node variable,
    // we hence use the '&' operator which returns the memory address of a variable. Then we just check if this memory
    // address equals 'this'. If they do, its the same object.
    if (this != &node && collision(*this, node) && !isDestroyed() && !node.isDestroyed())
        collisionPairs.insert(std::minmax(this, &node));
    
    for (Ptr& child : mChildren)
        child->checkNodeCollision(node, collisionPairs);
}

void SceneNode::removeWrecks()
{
    auto wreckfieldBegin = std::remove_if(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
    
    mChildren.erase(wreckfieldBegin, mChildren.end());
    
    std::for_each(mChildren.begin(), mChildren.end(), std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
    return false;
}

bool collision(const SceneNode& lhs, const SceneNode& rhs)
{
    return lhs.getBoundingRect().intersects(rhs.getBoundingRect());
}

float distance(const SceneNode& lhs, const SceneNode& rhs)
{
    return length(lhs.getWorldPosition() - rhs.getWorldPosition());
}
