/// @file   TamyEditor/RPBRenderTarget.h
/// @brief  rendering pipeline block representing a render target node
#pragma once

#include "RenderingPipelineBlock.h"
#include "core/ResourceHandle.h"
#include <QPen>


///////////////////////////////////////////////////////////////////////////////

class RPRenderTargetNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block representing a render target node.
 */
class RPBRenderTarget : public RenderingPipelineBlock
{
   DECLARE_CLASS( RPBRenderTarget )

private:
   static QPen                                        s_dragPen;

   RPRenderTargetNode*                                m_nodePtr;
   TResourceHandle< RPRenderTargetNode >*             m_node;

   bool                                               m_renderTargetDraggedIn;

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBRenderTarget();

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBRenderTarget( RPRenderTargetNode& node );
   ~RPBRenderTarget();

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
   virtual QColor getBgColor() const { return QColor( 186, 166, 221 ); }
   virtual QPen getBorderPen() const;

   void dragEnterEvent( QGraphicsSceneDragDropEvent *event );
   void dragMoveEvent( QGraphicsSceneDragDropEvent *event );
   void dragLeaveEvent( QGraphicsSceneDragDropEvent *event );
   void dropEvent( QGraphicsSceneDragDropEvent *event );
};

///////////////////////////////////////////////////////////////////////////////
