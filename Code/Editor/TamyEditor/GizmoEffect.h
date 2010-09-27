#pragma once

/// @file   TamyEditor\GizmoEffect.h
/// @brief  an effect visualizing an object selection gizmo


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class Camera;
class Node;
class EffectShader;
class GeometryResource;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect rendering a gizmo.
 */
class GizmoEffect
{
private:
   EffectShader*     m_effect;
   Camera&           m_camera;
   Node&             m_renderedNode;

public:
   /**
    * Constructor.
    * 
    * @param rm            resources manager
    * @param camera
    * @param renderedNode
    */
   GizmoEffect( ResourcesManager& rm, Camera& camera, Node& renderedNode );

   /**
    * Renders the geometry.
    *
    * @param geometry
    */
   void render( GeometryResource& geometry );
};

///////////////////////////////////////////////////////////////////////////////
