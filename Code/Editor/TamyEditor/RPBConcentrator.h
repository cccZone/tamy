/// @file   TamyEditor/RPBConcentrator.h
/// @brief  rendering pipeline block representing a concentrator node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPConcentratorNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a concentrator node.
 */
class RPBConcentrator : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBConcentrator )

private:
   RPConcentratorNode*                                 m_nodePtr;
   TResourceHandle< RPConcentratorNode >*              m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBConcentrator() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBConcentrator( RPConcentratorNode& node );
   ~RPBConcentrator();

   // -------------------------------------------------------------------------
   // GraphBlock implementation
   // -------------------------------------------------------------------------
   Object& getNode();
   void initialize( RenderingPipeline& parentResource );

protected:
   /**
    * Returns the shape of the block.
    */
   virtual Shape getShape() const { return GBS_ROUNDED; }
   virtual QColor getBgColor() const { return QColor( 224, 224, 166 ); }
};

///////////////////////////////////////////////////////////////////////////////
