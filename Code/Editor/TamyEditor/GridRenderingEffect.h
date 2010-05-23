#pragma once

/// @file   TamyEditor\GridRenderingEffect.h
/// @brief  an effect for simple grid rendering

#include "core-Renderer\ShaderEffect.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
class Node;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect rendering a simple grid
 */
class GridRenderingEffect : public ShaderEffect
{
private:
   Camera*     m_camera;
   Node*       m_renderedNode;
   Shader*     m_shader;

public:
   GridRenderingEffect( Renderer& renderer );
   ~GridRenderingEffect();

protected:
   void onBeginRendering();
   void onAttached(Entity& parent);
   void onDetached(Entity& parent);
   void onAttached(Model& hostModel);
   void onDetached(Model& hostModel);
   void onComponentAdded( Component< Model >& component );
};

///////////////////////////////////////////////////////////////////////////////
