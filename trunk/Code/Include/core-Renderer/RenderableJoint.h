#pragma once

/// @file   core-Renderer\Mesh.h
/// @brief  a joint in the spatial entities hierarchy

#include "core-Renderer\SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class can help create a hierarchical renderable. It's not rendered
 * by itself, however it can aggregate other SpatialEntities and thus
 * have multiple renderables attached to it.
 */
class RenderableJoint : public SpatialEntity
{
   DECLARE_CLASS(RenderableJoint)

public:
   /**
    * Constructor.
    */
   RenderableJoint() : SpatialEntity("RenderableJoint") {}
};

///////////////////////////////////////////////////////////////////////////////
