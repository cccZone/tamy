#pragma once

/// @file   core-Renderer\SpatialEntity.h
/// @brief  abstract entity that has a location in 3D space

#include "core\Node.h"
#include "core-MVC\Entity.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * Abstract entity that has a location in 3D space.
 */
class SpatialEntity : public Entity, public Node
{
   DECLARE_ABSTRACT_CLASS(SpatialEntity)

public:
   virtual ~SpatialEntity();

protected:
   /**
    * Constructor.
    *
    * @param name    node name
    */
   SpatialEntity(const std::string& name);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onSaveEntity(Serializer& serializer) {}
   void onLoadEntity(Serializer& serializer) {}
   void onChildAttached(Entity& child);
   void onChildDetached(Entity& child);
   void onAttached(Entity& entity) {}
   void onDetached(Entity& entity) {}
   void onAttached(Model& hostModel) {}
   void onDetached(Model& hostModel) {}
};

///////////////////////////////////////////////////////////////////////////////
