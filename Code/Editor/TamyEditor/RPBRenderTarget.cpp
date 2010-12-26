#include "RPBRenderTarget.h"
#include "core-Renderer/RPRenderTargetNode.h"
#include "RenderTargetMimeData.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPBRenderTarget, RenderingPipelineBlock );
   PROPERTY( TResourceHandle< RPRenderTargetNode >*, m_node );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

QPen RPBRenderTarget::s_dragPen( QBrush( QColor( 100, 100, 255 ) ), 3.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

///////////////////////////////////////////////////////////////////////////////

RPBRenderTarget::RPBRenderTarget() 
   : m_nodePtr( NULL )
   , m_node( NULL )
   , m_renderTargetDraggedIn( false ) 
{
   // we're gonna accept drops from the render targets list
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

RPBRenderTarget::RPBRenderTarget( RPRenderTargetNode& node )
   : m_nodePtr( &node )
   , m_renderTargetDraggedIn( false )
{
   setCaption( "RenderTarget" );

   addSocket().initialize( GBSP_LEFT, "Input" );
   addSocket().initialize( GBSP_RIGHT, "Output" );

   // we're gonna accept drops from the render targets list
   setAcceptDrops( true );
}

///////////////////////////////////////////////////////////////////////////////

RPBRenderTarget::~RPBRenderTarget()
{
   delete m_node;
   m_node = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPBRenderTarget::initialize( RenderingPipeline& parentResource )
{
   m_node = new TResourceHandle< RPRenderTargetNode >( *m_nodePtr );
   m_renderTargetDraggedIn = false;
}

///////////////////////////////////////////////////////////////////////////////

Object& RPBRenderTarget::getNode() 
{ 
   return m_node->get();
}

///////////////////////////////////////////////////////////////////////////////

void RPBRenderTarget::dragEnterEvent( QGraphicsSceneDragDropEvent *event )
{
   m_renderTargetDraggedIn = true;
   event->acceptProposedAction();

   update();
}

///////////////////////////////////////////////////////////////////////////////

void RPBRenderTarget::dragMoveEvent( QGraphicsSceneDragDropEvent *event )
{
   event->acceptProposedAction();
}

///////////////////////////////////////////////////////////////////////////////

void RPBRenderTarget::dragLeaveEvent( QGraphicsSceneDragDropEvent *event )
{
   m_renderTargetDraggedIn = false;
   event->acceptProposedAction();
   update();
}

///////////////////////////////////////////////////////////////////////////////

void RPBRenderTarget::dropEvent( QGraphicsSceneDragDropEvent *event )
{
   // decode the dragged data
   const QMimeData* mimeData = event->mimeData();

   std::string selectedTargetID;
   RenderTargetMimeData dataEncoder( selectedTargetID );
   dataEncoder.load( *mimeData );

   if ( selectedTargetID.empty() == false )
   {
      RPRenderTargetNode& node = m_node->get();
      node.setRenderTargetID( selectedTargetID );
   }

   // process the event
   m_renderTargetDraggedIn = false;
   event->acceptProposedAction();
   update();
}

///////////////////////////////////////////////////////////////////////////////

QPen RPBRenderTarget::getBorderPen() const
{
   return m_renderTargetDraggedIn ? s_dragPen : __super::getBorderPen();
}

///////////////////////////////////////////////////////////////////////////////
