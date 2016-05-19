//
//  Component.cpp
//  SpaceWars
//
//  Created by Erica Ko on 19/05/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#include "Component.hpp"

namespace GUI
{
    Component::Component()
    : mIsSelected(false)
    , mIsActive(false)
    {
    }
    
    Component::~Component()
    {
        
    }
    
    bool Component::isSelected() const
    {
        return mIsSelected;
    }
    
    void Component::select()
    {
        mIsSelected = true;
    }
    
    void Component::deselect()
    {
        mIsSelected = false;
    }
    
    bool Component::isActive() const
    {
        return mIsActive;
    }
    
    void Component::activate()
    {
        mIsActive = true;
    }
    
    void Component::deactivate()
    {
        mIsActive = false;
    }
}