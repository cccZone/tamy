#pragma once

/// @file   core-MVC\SpatialEntity.h
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
   DECLARE_CLASS()

public:
   /**
    * Constructor.
    *
    * @param name    node name
    */
   SpatialEntity( const std::string& name = "" );

   /**
    * Copy constructor.
    */
   SpatialEntity( const SpatialEntity& rhs );

   virtual ~SpatialEntity();

protected:
   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void onChildAttached( Entity& child );
   void onChildDetached( Entity& child );
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////
