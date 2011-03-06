/// @file   TamyEditor/RPBStart.h
/// @brief  rendering pipeline block representing a rendering process adapter node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPAdapterNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a rendering process start node.
 */
class RPBAdapter : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBAdapter )

private:
   RPAdapterNode*                               m_nodePtr;
   TResourceHandle< RPAdapterNode >*            m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBAdapter() : RenderingPipelineBlock(), m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBAdapter( RPAdapterNode& node );
   ~RPBAdapter();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_CIRCLE; }
   virtual QColor getBgColor() const { return QColor( 183, 145, 75 ); }
};

///////////////////////////////////////////////////////////////////////////////
