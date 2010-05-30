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
   Node*       m_renderedNode;

public:
   /**
    * Constructor.
    * 
    * @param rm            resources manager
    * @param camera
    */
   GizmoEffect( ResourcesManager& rm, Camera& camera );

protected:
   void onBeginRendering();

   // -------------------------------------------------------------------------
   // Entity implementation 
   // -------------------------------------------------------------------------
   void onAttached(Entity& parent);
   void onDetached(Entity& parent);
};

///////////////////////////////////////////////////////////////////////////////
