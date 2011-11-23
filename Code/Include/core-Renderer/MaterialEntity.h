/// @file   core-Renderer/MaterialEntity.h
/// @brief  entity that allows to embed a material in a scene graph
#pragma once

#include "core-MVC/Entity.h"
#include "core-Renderer/RenderState.h"


///////////////////////////////////////////////////////////////////////////////

class Material;
class RuntimeData;

///////////////////////////////////////////////////////////////////////////////

/**
 * Entity that allows to embed a material in a scene graph.
 */
class MaterialEntity : public Entity, public TRenderState< MaterialEntity >
{
   DECLARE_CLASS( MaterialEntity )

private:
   // static data
   Material*            m_material;

   // runtime data
   RuntimeDataBuffer*   m_dataBuf;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   MaterialEntity( const std::string& name = "" );
   ~MaterialEntity();

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
   void onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
   bool onEquals( const MaterialEntity& rhs ) const;
   bool onLess( const MaterialEntity& rhs ) const;

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPrePropertyChanged( Property& property );
   void onPropertyChanged( Property& property );

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;

private:
   void initializeMaterial();
   void deinitializeMaterial();
};

///////////////////////////////////////////////////////////////////////////////
