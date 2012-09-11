/// @file   TamyEditor/MBSpatialEntity.h
/// @brief  material pipeline block representing a node that provides info about parent spatial entity
#pragma once

#include "core-Renderer/MNSpatialEntity.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////


class MBSpatialEntity : public TPipelineBlock< MNSpatialEntity, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBSpatialEntity() : TPipelineBlock< MNSpatialEntity, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBSpatialEntity( MNSpatialEntity& node );
};

///////////////////////////////////////////////////////////////////////////////
