/// @file   core-AI/BoneEntity.h
/// @brief  entity composing a skeleton
#pragma once

#include "core-MVC/SpatialEntity.h"
#include "core/IDebugDraw.h"
#include "core-MVC/ModelComponent.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Entity composing a skeleton.
 */
class BoneEntity : public SpatialEntity, public IDebugDrawable
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

   // -------------------------------------------------------------------------
   // IDebugDrawable representation
   // -------------------------------------------------------------------------
   void onDebugRender( Renderer& renderer ) const;

protected:
   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   Entity* cloneSelf() const;
};

///////////////////////////////////////////////////////////////////////////////
