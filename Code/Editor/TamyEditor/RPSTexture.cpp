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

BEGIN_OBJECT( RPSTextureOutput, RenderingPipelineSocketRepresentation );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

QPen RPSTextureOutput::s_dragPen( QBrush( QColor( 100, 100, 255 ) ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

///////////////////////////////////////////////////////////////////////////////

RPSTextureOutput::RPSTextureOutput() 
   : RenderingPipelineSocketRepresentation()
   , m_renderTargetDraggedIn( false )
{
   // we're gonna accept drops from the render targets list
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

RPSTextureOutput::RPSTextureOutput( RPTextureOutput& socket ) 
   : RenderingPipelineSocketRepresentation( socket ) 
   , m_renderTargetDraggedIn( false )
{
   // we're gonna accept drops from the render targets list
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

void RPSTextureOutput::dragEnterEvent( QGraphicsSceneDragDropEvent *event )
{
   m_renderTargetDraggedIn = true;
   event->acceptProposedAction();

   update();
}

///////////////////////////////////////////////////////////////////////////////

void RPSTextureOutput::dragMoveEvent( QGraphicsSceneDragDropEvent *event )
{
   event->acceptProposedAction();
}

///////////////////////////////////////////////////////////////////////////////

void RPSTextureOutput::dragLeaveEvent( QGraphicsSceneDragDropEvent *event )
{
   m_renderTargetDraggedIn = false;
   event->acceptProposedAction();
   update();
}

///////////////////////////////////////////////////////////////////////////////

void RPSTextureOutput::dropEvent( QGraphicsSceneDragDropEvent *event )
{
   // decode the dragged data
   const QMimeData* mimeData = event->mimeData();

   std::string selectedTargetID;
   RenderTargetMimeData dataEncoder( selectedTargetID );
   dataEncoder.load( *mimeData );

   if ( selectedTargetID.empty() == false )
   {
      RenderingPipelineNode& parentNode = dynamic_cast< RenderingPipelineNode& >( getParentBlock().getNode() );
      RPTextureOutput* socket = dynamic_cast< RPTextureOutput* >( parentNode.findOutput( getSocketName() ) );

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

QPen RPSTextureOutput::getBorderPen() const
{
   return m_renderTargetDraggedIn ? s_dragPen : __super::getBorderPen();
}

///////////////////////////////////////////////////////////////////////////////
