#pragma once

/// @file   core-Scene\WorldEntity.h
/// @brief  a camera description

#include "core-Scene\Entity.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a directional light.
 */
struct WorldEntity : public Entity
{
public:
   AABoundingBox m_size;

public:
   /**
    * Default constructor.
    */
   WorldEntity();

   /**
    * Constructor.
    *
    * @param size      world size
    */
   WorldEntity(const AABoundingBox& size);

   // -------------------------------------------------------------------------
   // Serializable implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
protected:
   void registerProperties();
};

///////////////////////////////////////////////////////////////////////////////
