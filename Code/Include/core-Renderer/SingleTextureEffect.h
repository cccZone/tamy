#pragma once

/// @file   core-Renderer\SingleTextureEffect.h
/// @brief  simple rendering effect

#include "core-MVC\Entity.h"
#include "core-Renderer\SurfaceProperties.h"
#include "core-Renderer\RenderState.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class EffectShader;
class Texture;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
* This effect will render the geometry using a single texture
* and a material definition.
*/
class SingleTextureEffect : public Entity, public TRenderState< SingleTextureEffect >
{
   DECLARE_CLASS( SingleTextureEffect )

private:
   SurfaceProperties    m_surfaceProperties;
   Texture*             m_texture;

   // runtime data
   EffectShader*        m_effect;
   SpatialEntity*       m_parentNode;

public:
   /**
    * Constructor.
    */
   SingleTextureEffect();

   /**
    * This method allows to set an arbitrary surface properties for the effect to use.
    * 
    * @param properties
    */
   void setSurfaceProperties( const SurfaceProperties& properties );

   /**
    * This method allows to set an arbitrary texture for the effect to use.
    * 
    * @param texture
    */
   void setTexture(Texture& texture);

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const SingleTextureEffect& rhs ) const;
   bool onLess( const SingleTextureEffect& rhs ) const;

protected:
   // -------------------------------------------------------------------------
   // Object implementation 
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Entity implementation 
   // -------------------------------------------------------------------------
   void onAttached( Entity& parent );
   void onDetached( Entity& parent );
   void onComponentAdded( Component< Model >& component );
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////
