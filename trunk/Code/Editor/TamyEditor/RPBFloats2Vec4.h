/// @file   TamyEditor/RPBFloats2Vec4.h
/// @brief  rendering pipeline block representing a node that packs 4 floats into a vector
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPFloats2Vec4Node;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that packs 4 floats into a vector.
 */
class RPBFloats2Vec4 : public TRenderingPipelineBlock< RPFloats2Vec4Node >
{
   DECLARE_CLASS( RPBFloats2Vec4 )

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBFloats2Vec4() : TRenderingPipelineBlock< RPFloats2Vec4Node >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBFloats2Vec4( RPFloats2Vec4Node& node );

protected:
   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_CIRCLE; }
   virtual QColor getBgColor() const { return QColor( 221, 200, 255 ); }
};

///////////////////////////////////////////////////////////////////////////////
