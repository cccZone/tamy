/// @file   TamyEditor/RPBStart.h
/// @brief  rendering pipeline block representing a rendering process start node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPStartNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a rendering process start node.
 */
class RPBStart : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBStart )

private:
   RPStartNode*                                 m_nodePtr;
   TResourceHandle< RPStartNode >*              m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBStart() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBStart( RPStartNode& node );
   ~RPBStart();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_RECTANGLE; }
   virtual QColor getBgColor() const { return QColor( 224, 207, 166 ); }
};

///////////////////////////////////////////////////////////////////////////////
