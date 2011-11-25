/// @file   TamyEditor/MBSurfaceProperties.h
/// @brief  material pipeline block representing an instance surface properties provider
#pragma once

#include "core-Renderer/MNSurfaceProperties.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing an instance surface properties provider.
 */
class MBSurfaceProperties : public TPipelineBlock< MNSurfaceProperties, MaterialNode >
{
   DECLARE_CLASS( MBSurfaceProperties )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBSurfaceProperties() : TPipelineBlock< MNSurfaceProperties, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBSurfaceProperties( MNSurfaceProperties& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 141, 93, 255 ); }
};

///////////////////////////////////////////////////////////////////////////////
