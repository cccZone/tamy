#pragma once

/// @file   TamyEditor\GizmoEffect.h
/// @brief  an effect visualizing an object selection gizmo

#include "core-Renderer\ShaderEffect.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class Camera;
class Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect rendering a gizmo.
 */
class GizmoEffect : public ShaderEffect
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
   GizmoEffect( ResourcesManager& rm, Camera& camera, Node& renderedNode );

protected:
   void onBeginRendering();
};

///////////////////////////////////////////////////////////////////////////////
