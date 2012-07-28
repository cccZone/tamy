/// @file   core-Renderer/PointLight.h
/// @brief  point light entity
#pragma once

#include "core-Renderer\Light.h"
#include "core-Renderer\RenderCommand.h"
#include "core\Color.h"
#include "core\UniqueObject.h"


///////////////////////////////////////////////////////////////////////////////

struct BoundingSphere;
class PixelShader;
class VertexShader;
class TriangleMesh;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a special kind of node that represents a point light in a scene
 */
class PointLight : public Light, public UniqueObject< PointLight >
{
   DECLARE_CLASS();

public:
   // static data
   Color             m_color;
   float             m_attenuation;
   float             m_radius;

   // runtime data
   BoundingSphere*   m_boundingSphere;
   VertexShader*     m_vertexShader;
   PixelShader*      m_pixelShader;
   TriangleMesh*     m_pointLightMesh;

public:
   PointLight( const std::string& name = "" );
   virtual ~PointLight();

   // -------------------------------------------------------------------------
   // Light implementation
   // -------------------------------------------------------------------------
   void render( Renderer& renderer, ShaderTexture* depthNormalsBufferTex );

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPropertyChanged( ReflectionProperty& property );

private:
   void initialize();
};

///////////////////////////////////////////////////////////////////////////////
