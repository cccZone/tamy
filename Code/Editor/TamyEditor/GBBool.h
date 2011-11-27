/// @file   TamyEditor/GBBool.h
/// @brief  geometry shader pipeline block representing a bool value provider node
#pragma once

#include "core-Renderer/GNBool.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Geometry shader pipeline block representing a bool value provider node.
 */
class GBBool : public TPipelineBlock< GNBool, GeometryShaderNode >
{
   DECLARE_CLASS( GBBool )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBBool() : TPipelineBlock< GNBool, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBBool( GNBool& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 141, 136, 167 ); }
};

///////////////////////////////////////////////////////////////////////////////
