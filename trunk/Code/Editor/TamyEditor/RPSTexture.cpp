#include "RPSTexture.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "RenderTargetMimeData.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSTextureInput, RenderingPipelineSocketRepresentation );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSRenderTargetOutput, RenderingPipelineSocketRepresentation );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

QPen RPSRenderTargetOutput::s_dragPen( QBrush( QColor( 100, 100, 255 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

///////////////////////////////////////////////////////////////////////////////

RPSRenderTargetOutput::RPSRenderTargetOutput() 
   : RenderingPipelineSocketRepresentation()
   , m_renderTargetDraggedIn( false )
{
   // we're gonna accept drops from the render targets list
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

RPSRenderTargetOutput::RPSRenderTargetOutput( RPRenderTargetOutput& socket ) 
   : RenderingPipelineSocketRepresentation( socket ) 
   , m_renderTargetDraggedIn( false )
{
   // we're gonna accept drops from the render targets list
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

void RPSRenderTargetOutput::dragEnterEvent( QGraphicsSceneDragDropEvent *event )
{
   m_renderTargetDraggedIn = true;
   event->acceptProposedAction();

   update();
}

///////////////////////////////////////////////////////////////////////////////

void RPSRenderTargetOutput::dragMoveEvent( QGraphicsSceneDragDropEvent *event )
{
   event->acceptProposedAction();
}

///////////////////////////////////////////////////////////////////////////////

void RPSRenderTargetOutput::dragLeaveEvent( QGraphicsSceneDragDropEvent *event )
{
   m_renderTargetDraggedIn = false;
   event->acceptProposedAction();
   update();
}

///////////////////////////////////////////////////////////////////////////////

void RPSRenderTargetOutput::dropEvent( QGraphicsSceneDragDropEvent *event )
{
   // decode the dragged data
   const QMimeData* mimeData = event->mimeData();

   std::string selectedTargetID;
   RenderTargetMimeData dataEncoder( selectedTargetID );
   dataEncoder.load( *mimeData );

   if ( selectedTargetID.empty() == false )
   {
      RenderingPipelineNode& parentNode = dynamic_cast< RenderingPipelineNode& >( getParentBlock().getNode() );
      RPRenderTargetOutput* socket = dynamic_cast< RPRenderTargetOutput* >( parentNode.findOutput( getSocketName() ) );

      if ( socket )
      {
         socket->setRenderTargetID( selectedTargetID );
      }
   }

   // process the event
   m_renderTargetDraggedIn = false;
   event->acceptProposedAction();
   update();
}

///////////////////////////////////////////////////////////////////////////////

QPen RPSRenderTargetOutput::getBorderPen() const
{
   return m_renderTargetDraggedIn ? s_dragPen : __super::getBorderPen();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSImageOutput, RenderingPipelineSocketRepresentation );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////