/// @file   TamyEditor/MBCamera.h
/// @brief  material pipeline block representing a camera node
#pragma once

#include "core-Renderer/MNCamera.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class MNCamera;

///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a camera node.
 */
class MBCamera : public TPipelineBlock< MNCamera, MaterialNode >
{
   DECLARE_CLASS( MBCamera )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBCamera() : TPipelineBlock< MNCamera, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBCamera( MNCamera& node );

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
