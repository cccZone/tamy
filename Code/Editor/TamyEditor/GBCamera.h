/// @file   TamyEditor/GBCamera.h
/// @brief  geometry shader pipeline block representing a camera node
#pragma once

#include "core-Renderer/GNCamera.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Geometry shader pipeline block representing a camera node.
 */
class GBCamera : public TPipelineBlock< GNCamera, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBCamera() : TPipelineBlock< GNCamera, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBCamera( GNCamera& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 248, 174, 91 ); }
};

///////////////////////////////////////////////////////////////////////////////
