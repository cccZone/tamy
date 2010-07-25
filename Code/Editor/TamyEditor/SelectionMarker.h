#pragma once

/// @file   TamyEditor\SelectionMarker.h
/// @brief  an effect visualizing the selected object

#include "core-Renderer\ShaderEffect.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class Camera;
class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect rendering a selection hull around an entity.
 */
class SelectionMarker : public ShaderEffect
{
private:
   Camera&     m_camera;
   Node&       m_renderedNode;

public:
   /**
    * Constructor.
    * 
    * @param rm            resources manager
    * @param camera
    * @param renderedNode
    */
   SelectionMarker( ResourcesManager& rm, Camera& camera, Node& renderedNode );

protected:
   void onBeginRendering();
};


///////////////////////////////////////////////////////////////////////////////