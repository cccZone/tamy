#pragma once

/// @file   core-Renderer\SingleTextureEffect.h
/// @brief  simple rendering effect

#include "core-MVC\Entity.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Renderable.h"


///////////////////////////////////////////////////////////////////////////////

class SpatialEntity;
class GeometryResource;
class EffectShader;
class Material;
class Texture;
class Camera;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
* This effect will render the geometry using a single texture
* and a material definition.
*/
class SingleTextureEffect : public Entity, public Renderable
{
   DECLARE_CLASS( SingleTextureEffect )

private:
   // instance data
   GeometryResource*    m_geometry;
   Material             m_material;
   Texture*             m_texture;

   // runtime data
   EffectShader*        m_effect;
   SpatialEntity*       m_parentNode;
   Camera*              m_camera;

   Attributes           m_attributes;

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

   /**
    * Sets the geometry the effect should use.
    *
    * @param geometry
    */
   void setGeometry( GeometryResource& geometry );

   // -------------------------------------------------------------------------
   // Renderable implementation
   // -------------------------------------------------------------------------
   void render();
   const Attributes& getAttributes() const { return m_attributes; }

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
   void onAttached( Model& hostModel );
   void onDetached( Model& hostModel );
   void onComponentAdded( Component< Model >& component );
};

///////////////////////////////////////////////////////////////////////////////
