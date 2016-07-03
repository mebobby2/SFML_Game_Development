//
//  NetworkNode.hpp
//  SpaceWars
//
//  Created by Bobby Lei on 3/7/16.
//  Copyright © 2016 mebobby. All rights reserved.
//

#ifndef NetworkNode_hpp
#define NetworkNode_hpp

#include "SceneNode.hpp"
#include "NetworkProtocol.hpp"

#include <queue>

class NetworkNode : public SceneNode
{
public:
    NetworkNode();
    
    void notifyGameAction(GameActions::Type type, sf::Vector2f position);
    bool pollGameAction(GameActions::Action& out);
    
    virtual unsigned int getCategory() const;
    
private:
    std::queue<GameActions::Action> mPendingActions;
};

#endif /* NetworkNode_hpp */
