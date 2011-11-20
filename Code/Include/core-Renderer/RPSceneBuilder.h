/// @file   core-Renderer/RPSceneBuilder.h
/// @brief  base class for the rendering pipeline scene builders.
#pragma once

#include "core/Object.h"
#include "core-Renderer/SceneRenderTreeBuilder.h"


///////////////////////////////////////////////////////////////////////////////

class RPSceneRenderNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Base class for the scene builders that can be embedded in the scene rendering block.
 */
class RPSceneBuilder : public Object, public SceneRenderTreeBuilder
{
   DECLARE_CLASS( RPSceneBuilder )

protected:
   // runtime data
   RPSceneRenderNode*      m_hostNode;

public:
   RPSceneBuilder();
   virtual ~RPSceneBuilder();

   /**
    * Called by the host scene render node so that the builder can create
    * the sockets it needs in order to work.
    *
    * @param hostNode
    */
   void defineSockets( RPSceneRenderNode& hostNode );

protected:
   /**
    * Override to add implementation-specific scokets to the host node.
    *
    * @param hostNode
    */
   virtual void onDefineSockets( RPSceneRenderNode& hostNode ) {}
};

///////////////////////////////////////////////////////////////////////////////
