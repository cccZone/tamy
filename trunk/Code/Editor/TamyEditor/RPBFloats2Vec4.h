/// @file   TamyEditor/RPBFloats2Vec4.h
/// @brief  rendering pipeline block representing a node that packs 4 floats into a vector
#pragma once

#include "core-Renderer/RPFloats2Vec4Node.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that packs 4 floats into a vector.
 */
class RPBFloats2Vec4 : public TPipelineBlock< RPFloats2Vec4Node, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBFloats2Vec4() : TPipelineBlock< RPFloats2Vec4Node, RenderingPipelineNode >() {}

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
