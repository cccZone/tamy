/// @file   TamyEditor/GBSpatialEntity.h
/// @brief  geometry shader pipeline block representing a node that provides info about parent spatial entity
#pragma once

#include "core-Renderer/GNSpatialEntity.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class GBSpatialEntity : public TPipelineBlock< GNSpatialEntity, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBSpatialEntity() : TPipelineBlock< GNSpatialEntity, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBSpatialEntity( GNSpatialEntity& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 166, 198, 224 ); }
};

///////////////////////////////////////////////////////////////////////////////
