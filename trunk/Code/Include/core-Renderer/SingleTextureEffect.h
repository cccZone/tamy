#pragma once

/// @file   core-Renderer\SingleTextureEffect.h
/// @brief  simple rendering effect

#include "core-MVC\Entity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\RenderState.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class EffectShader;
class Material;
class Texture;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

// TODO: setting the renderState in case based on the texture used etc.

/**
* This effect will render the geometry using a single texture
* and a material definition.
*/
class SingleTextureEffect : public Entity, public RenderState
{
   DECLARE_CLASS( SingleTextureEffect )

private:
   Material             m_material;
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
    * This method allows to set an arbitrary material for the effect to use.
    * 
    * @param material
    */
   void setMaterial(const Material& material);

   /**
    * This method allows to set an arbitrary texture for the effect to use.
    * 
    * @param texture
    */
   void setTexture(Texture& texture);

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );

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
};

///////////////////////////////////////////////////////////////////////////////
