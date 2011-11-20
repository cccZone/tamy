/// @file   TamyEditor/RPBVec4.h
/// @brief  rendering pipeline block representing a vector provider node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPVec4Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a vector provider node.
 */
class RPBVec4 : public TRenderingPipelineBlock< RPVec4Node >
{
   DECLARE_CLASS( RPBVec4 )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBVec4() : TRenderingPipelineBlock< RPVec4Node >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBVec4( RPVec4Node& node );

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
