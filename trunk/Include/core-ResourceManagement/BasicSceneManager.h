#pragma once

#include "core-ResourceManagement\SceneManager.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager doesn't do much - it assigns all the nodes
 * to its root node, so there's not much hierarchy here.
 */
class BasicSceneManager : public SceneManager
{
public:
   void addNode(Node* newNode);
};

///////////////////////////////////////////////////////////////////////////////
