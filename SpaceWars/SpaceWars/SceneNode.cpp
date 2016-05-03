//
//  SceneNode.cpp
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#include "SceneNode.hpp"
#include <cassert>

SceneNode::SceneNode()
: mChildren()
, mParent(nullptr)
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
    // A pointer stores the memory address of a object it's pointing to.
    
    // References in C++ are 'aliases' for another variable and the main reason they are added
    // to the language is to support pass by reference in function calls. That is why C++
    // makes it nice to work with references in that you do not need to explicitly dereference
    // them, using *, like you do when using pointers.
    // &node returns the memory address of the node which the reference is pointing to.
    
    // Therefore, p.get() == &node is testing whether the address stored by the pointer p.get()
    // is equal to the memory address of the node referenced by &node.
    // We need to explicitly use '&' in front of node in '&node' because C++ implicitly dereferences
    // references for us, but we do not want the value the &node is pointed to, we want the address,
    // hence we need to explicitly get the memory address with the '&' operator.
    // Since pointers are not implicitly deferences, p.get() will return the address of the node as
    // opposed to returning the valye at that address.
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                              [&] (Ptr& p) -> bool { return p.get() == &node; });
    assert(found != mChildren.end());
    
    Ptr result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);
    return result;
}