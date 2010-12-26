/// @file   TamyEditor/RPBSceneRender.h
/// @brief  rendering pipeline block representing a node that renders scenes
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class RPSceneRenderNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a node that renders scenes.
 */
class RPBSceneRender : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBSceneRender )

private:
   RPSceneRenderNode*                                 m_nodePtr;
   TResourceHandle< RPSceneRenderNode >*              m_node;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBSceneRender() : m_nodePtr( NULL ), m_node( NULL ) {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBSceneRender( RPSceneRenderNode& node );
   ~RPBSceneRender();

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
   virtual QColor getBgColor() const { return QColor( 166, 198, 224 ); }
};

///////////////////////////////////////////////////////////////////////////////
