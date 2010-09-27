/// @file   TamyEditor\GridRenderingEffect.h
/// @brief  an effect for simple grid rendering

#pragma once

#include "core-Renderer\Renderable.h"
#include "core-MVC.h"


///////////////////////////////////////////////////////////////////////////////

class GeometryResource;
class Camera;
class ResourcesManager;
class EffectShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * An effect rendering a simple grid
 */
class GridRenderingEffect : public Entity, public Renderable
{
private:
   GeometryResource* m_geometry;
   Camera&           m_camera;
   EffectShader*     m_shader;

   Attributes        m_attributes;

public:
   GridRenderingEffect( GeometryResource* geometry, ResourcesManager& rm, Camera& camera );
   ~GridRenderingEffect();

   // -------------------------------------------------------------------------
   // Renderable representation
   // -------------------------------------------------------------------------
   void render();
   const Attributes& getAttributes() const { return m_attributes; }
};

///////////////////////////////////////////////////////////////////////////////
