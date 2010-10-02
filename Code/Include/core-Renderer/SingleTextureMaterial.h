#pragma once

/// @file   core-Renderer\SingleTextureMaterial.h
/// @brief  a material that can include up to one texture

#include "core-MVC\Entity.h"
#include "core-Renderer\EffectAttribute.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Material.h"
#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Geometry;
class PixelShader;
class Camera;
class Texture;

///////////////////////////////////////////////////////////////////////////////

/**
 * An atomic object that can be rendered Such an object consists of geometry
 * and effects that render the object in a particular way.
 */
class SingleTextureMaterial : public Entity, public Renderable
{
   DECLARE_CLASS( SingleTextureMaterial )

private:
   PixelShader*         m_shader;
   Material             m_material;
   Texture*             m_texture;

   // runtime data
   Camera*              m_camera;
   Geometry*            m_geometry;
   Attributes           m_attributes;

public:
   /**
    * Default constructor.
    */
   SingleTextureMaterial( const std::string& name = "" );
   ~SingleTextureMaterial();

   /**
    * This method allows to set an arbitrary material for the effect to use.
    * 
    * @param material
    */
   inline void setMaterial( const Material& material ) { m_material = material; }

   /**
    * This method allows to set an arbitrary texture for the effect to use.
    * 
    * @param texture
    */
   inline void setTexture( Texture& texture ) { m_texture = &texture; }

   // -------------------------------------------------------------------------
   // Renderable implementation
   // -------------------------------------------------------------------------
   void render();
   inline const Attributes& getAttributes() const { return m_attributes; }

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onAttached( Entity& parent );
   void onDetached( Entity& parent );
   void onComponentAdded( Component< Model >& component );
};

///////////////////////////////////////////////////////////////////////////////
