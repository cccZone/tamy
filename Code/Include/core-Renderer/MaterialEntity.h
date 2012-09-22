/// @file   core-Renderer/MaterialEntity.h
/// @brief  entity that allows to embed a material in a scene graph
#pragma once

#include "core-MVC/Entity.h"
#include "core-Renderer/RenderState.h"
#include "core-Renderer/SurfaceProperties.h"
#include "core/ReflectionEnum.h"
#include "core/Observer.h"


///////////////////////////////////////////////////////////////////////////////

class MaterialInstance;

///////////////////////////////////////////////////////////////////////////////

/**
 * Entity that allows to embed a material in a scene graph.
 */
class MaterialEntity :  public Entity, public TRenderState< MaterialEntity >
{
   DECLARE_CLASS()

public:
   // static data
   MaterialInstance*                m_material;

public:
   /**
    * Constructor.
    *
    * @param name
    */
   MaterialEntity( const std::string& name = "" );
   ~MaterialEntity();

   /**
    * Sets a new material instance on the entity.
    *
    * @param materialInstance
    */
   void setMaterial( MaterialInstance* materialInstance );

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer ) const;
   void onPostRender( Renderer& renderer ) const;
   bool onEquals( const MaterialEntity& rhs ) const;
   bool onLess( const MaterialEntity& rhs ) const;

protected:

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////
