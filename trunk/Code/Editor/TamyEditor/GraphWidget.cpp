#include "GraphWidget.h"
#include "core.h"
#include <QMouseEvent>
#include <QMenu.h>
#include <QTimer>



// TODO !!!!!!!!!!!!!!!! : 
//  1.) pamietanie layout'u node'ow
//  2.) naglowek node'a
//  3.) ogladanie propertiesow node'a


///////////////////////////////////////////////////////////////////////////////

GraphWidget::GraphWidget( QWidget* parent )
   : QGraphicsView( parent )
   , m_selectionMode( SM_EXCLUSIVE )
   , m_isDraggingBlocks( false )
{
   m_scene = new QGraphicsScene( this );
   setScene( m_scene );

   m_dragTimer = new QTimer( this );
   connect( m_dragTimer, SIGNAL( timeout() ), this, SLOT( onDragTimerTimeout() ) );
}

///////////////////////////////////////////////////////////////////////////////

GraphWidget::~GraphWidget()
{
   unsigned int count = m_blocks.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_blocks[i];
   }
   m_blocks.clear();

   delete m_scene;
   m_scene = NULL;

   delete m_dragTimer;
   m_dragTimer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::addNodeAction( const Class& nodeType, const QPointF& addingPos )
{
   void* node = nodeType.instantiate< void >();
   emit addNode( node );

   GraphBlock* representation = createBlock( nodeType, node );
   ASSERT_MSG( representation != NULL, "Node representation was not created" );
   if ( representation )
   {
      m_blocks.push_back( representation );
      representation->setPos( addingPos );
      m_scene->addItem( representation );
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::removeNodeAction( QGraphicsItem* item )
{
   m_scene->removeItem( item );

   GraphBlock* block = dynamic_cast< GraphBlock* >( item );
   if ( !block )
   {
      return;
   }

   std::vector< GraphBlock* >::iterator it = std::find( m_blocks.begin(), m_blocks.end(), block );
   ASSERT_MSG( it != m_blocks.end(), "Attempting to remove an unregistered graph block" );
   m_blocks.erase( it );

   void* node = block->getNode();
   delete block;

   emit removeNode( node );
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::handleBlockSelection( const QPointF& scenePos )
{
   // select a block if the mouse points at any
   QGraphicsItem* pointedBlock = m_scene->itemAt( scenePos, transform() );

   switch( m_selectionMode )
   {
   case SM_EXCLUSIVE:
      {
         m_scene->clearSelection();
         if ( pointedBlock )
         {
            pointedBlock->setSelected( true );
         }
         break;
      }

   case SM_ADDITIVE:
      {
         if ( pointedBlock )
         {
            pointedBlock->setSelected( !pointedBlock->isSelected() );
         }
      }
   }
   
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::mousePressEvent( QMouseEvent* event )
{
   switch( event->button() )
   {
   case Qt::LeftButton:
      {
         // handle block selection
         QPointF localActivationPoint = mapToScene( event->pos() );
         handleBlockSelection( localActivationPoint );

         // activate a timer, which, if not turned off promptly, will trigger a block dragging mode
         m_dragTimer->start( 100 );
         break;
      }

   case Qt::RightButton:
      {
         showPopupMenu( event->pos() );
         break;
      }
   }

   // accept the event
   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::mouseReleaseEvent( QMouseEvent* event )
{
   if ( event->button() == Qt::LeftButton )
   {
      m_dragTimer->stop();
      m_isDraggingBlocks = false;
   }

   // accept the event
   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::mouseMoveEvent( QMouseEvent* event )
{
   if ( m_isDraggingBlocks )
   {
      // handle blocks dragging action
      QPointF dPos = mapToScene( event->pos() ) - mapToScene( m_prevMousePos );

      QList< QGraphicsItem* > items = m_scene->selectedItems();
      foreach( QGraphicsItem* item, items )
      {
         item->setPos( item->pos() + dPos );
      }

      m_scene->update();
   }

   // memorize the mouse pos
   m_prevMousePos = event->pos();

   // accept the event
   event->accept();
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::onDragTimerTimeout()
{
   m_isDraggingBlocks = true;
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::keyPressEvent( QKeyEvent* event )
{
   if ( event->key() == Qt::Key_Shift )
   {
      // shift key changes the selection mode
      m_selectionMode = SM_ADDITIVE;
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::keyReleaseEvent( QKeyEvent* event )
{
   if ( event->key() == Qt::Key_Shift )
   {
      // shift key changes the selection mode
      m_selectionMode = SM_EXCLUSIVE;
   }
}

///////////////////////////////////////////////////////////////////////////////

void GraphWidget::showPopupMenu( const QPoint& activationPoint )
{
   QMenu* popupMenu = new QMenu( this );

   QPointF scenePos = mapToScene( activationPoint );

   QGraphicsItem* pointedBlock = m_scene->itemAt( scenePos, transform() );
   if ( pointedBlock == NULL )
   {
      // we clicked an empty spot - show a menu for adding new nodes
      QMenu* addMenu = popupMenu->addMenu( "Add" );

      std::vector< Class > classes;
      emit getNodesClasses( classes );
      for( std::vector< Class >::const_iterator it = classes.begin(); it != classes.end(); ++it )
      {
         const Class& nodeClass = *it;

         QAction* addNodeAction = new GraphNodeCreationAction( *this, nodeClass, scenePos );
         addMenu->addAction( addNodeAction );
      }
   }
   
   // if there are blocks selected, show additional options
   QList< QGraphicsItem* > selectedItems = m_scene->selectedItems();
   if ( selectedItems.isEmpty() == false )
   {
      // an option for removing existing nodes
      QAction* removeNodesAction = new GraphNodeRemoveAction( *this, selectedItems );
      popupMenu->addAction( removeNodesAction );

      // TODO: kopiowanie node'ow
      /*
      // an option for copying existing nodes
      QAction* copyNodesAction = new GraphNodeCopyAction( *this, selectedItems );
      popupMenu->addAction( copyNodesAction );

      // an option for cutting existing nodes
      QAction* cutNodesAction = new GraphNodeCutAction( *this, selectedItems );
      popupMenu->addAction( cutNodesAction );
      */
   }
  
   // inform about a menu being shown
   emit popupMenuShown( *popupMenu );

   // display the menu
   popupMenu->popup( mapToGlobal( activationPoint ) );
}

///////////////////////////////////////////////////////////////////////////////

GraphBlock* GraphWidget::createBlock( const Class& nodeType, void* node )
{
   NodeAssociacion* associacion = findAssociacion( nodeType );
   if ( !associacion )
   {
      return NULL;
   }
   else
   {
      GraphBlock* block = new GraphBlock( associacion->m_shape, associacion->m_bgColor, node );
      m_scene->addItem( block );
      return block;
   }
}

///////////////////////////////////////////////////////////////////////////////

GraphWidget::NodeAssociacion* GraphWidget::findAssociacion( const Class& nodeType )
{
   unsigned int count = m_associacions.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_associacions[i] == nodeType )
      {
         return &m_associacions[i];
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

GraphBlock* GraphWidget::findBlock( const QPointF& scenePos )
{
   unsigned int count = m_blocks.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_blocks[i]->doesOverlap( scenePos ) )
      {
         return m_blocks[i];
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GraphNodeCreationAction::GraphNodeCreationAction( GraphWidget& parent, const Class& type, const QPointF& pos )
   : QAction( type.getShortName().c_str(), &parent )
   , m_parent( parent )
   , m_type( type )
   , m_pos( pos )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void GraphNodeCreationAction::onTriggered()
{
   m_parent.addNodeAction( m_type, m_pos );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

GraphNodeRemoveAction::GraphNodeRemoveAction( GraphWidget& parent, const QList< QGraphicsItem* >& items )
   : QAction( "Remove", &parent )
   , m_parent( parent )
   , m_items( items )
{
   connect( this, SIGNAL( triggered() ), this, SLOT( onTriggered() ) );
}

///////////////////////////////////////////////////////////////////////////////

void GraphNodeRemoveAction::onTriggered()
{
   foreach( QGraphicsItem* item, m_items )
   {
      m_parent.removeNodeAction( item );
   }
}

///////////////////////////////////////////////////////////////////////////////
