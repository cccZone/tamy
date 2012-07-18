/// @file   core-AI/BoneEntity.h
/// @brief  entity composing a skeleton
#pragma once

#include "core-MVC/SpatialEntity.h"
#include "core-MVC/ModelComponent.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Entity composing a skeleton.
 */
class BoneEntity : public SpatialEntity
{
   DECLARE_CLASS();

public:
   /**
    * Constructor.
    *
    * @param boneName
    */
   BoneEntity( const std::string& boneName = "" );
   ~BoneEntity();

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////
