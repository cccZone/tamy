/// @file   TamyEditor/RPBDepthNormals.h
/// @brief  rendering pipeline block representing a node that renders scene depth and normals
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPDepthNormalsNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders scenes.
 */
class RPBDepthNormals : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBDepthNormals )

private:
   RPDepthNormalsNode*                                m_nodePtr;
   TResourceHandle< RPDepthNormalsNode >*             m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBDepthNormals() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBDepthNormals( RPDepthNormalsNode& node );
   ~RPBDepthNormals();

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
   virtual QColor getBgColor() const { return QColor( 186, 208, 234 ); }
};

///////////////////////////////////////////////////////////////////////////////
