#pragma once

/// @file   core-Renderer\SingleTextureMaterial.h
/// @brief  a material that can include up to one texture

#include "core-MVC\Entity.h"
#include "core-Renderer\SurfaceProperties.h"
#include "core-Renderer\RenderState.h"
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class Texture;
class RenderableTexture;

///////////////////////////////////////////////////////////////////////////////

/**
 * An atomic object that can be rendered Such an object consists of geometry
 * and effects that render the object in a particular way.
 */
class SingleTextureMaterial : public Entity, public TRenderState< SingleTextureMaterial >
{
   DECLARE_CLASS()

private:
   PixelShader*         m_shader;
   SurfaceProperties    m_surfaceProperties;
   Texture*             m_texture;

   RenderableTexture*   m_renderableTexture;

public:
   /**
    * Default constructor.
    */
   SingleTextureMaterial( const std::string& name = "" );
   ~SingleTextureMaterial();

   /**
    * This method allows to set arbitrary surface properties for the effect to use.
    * 
    * @param material
    */
   inline void setSurfaceProperties( const SurfaceProperties& properties ) { m_surfaceProperties = properties; }

   /**
    * This method allows to set an arbitrary texture for the effect to use.
    * 
    * @param texture
    */
   inline void setTexture( Texture& texture ) { m_texture = &texture; }

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const SingleTextureMaterial& rhs ) const;
   bool onLess( const SingleTextureMaterial& rhs ) const;

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onComponentAdded( Component< Model >& component );
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////
