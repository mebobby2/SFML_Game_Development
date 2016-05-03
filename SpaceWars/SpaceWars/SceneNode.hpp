//
//  SceneNode.h
//  SpaceWars
//
//  Created by Bobby Lei on 3/05/16.
//  Copyright (c) 2016 mebobby. All rights reserved.
//

#ifndef __SpaceWars__SceneNode__
#define __SpaceWars__SceneNode__

#include <vector>
#include <memory>

class SceneNode
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;
    
    public:
        SceneNode();
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);
    
    private:
        std::vector<Ptr> mChildren;
        SceneNode* mParent;
};

#endif /* defined(__SpaceWars__SceneNode__) */
