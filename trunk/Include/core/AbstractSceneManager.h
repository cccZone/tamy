#pragma once

#include "core\SceneManager.h"
#include "core\NodeVisitor.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a leaf scene manager - it implements the management
 * of a single aspect of a scene - audio, visual, AI etc..
 */
class AbstractSceneManager : public SceneManager, public NodeVisitor
{
public:
   virtual ~AbstractSceneManager() {}
};

///////////////////////////////////////////////////////////////////////////////
