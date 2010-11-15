#include "RenderingPipelineEditor.h"
#include "core-Renderer.h"
#include "GraphWidget.h"
#include "GraphBlock.h"
#include <QCloseEvent>
#include <QMessageBox.h>


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineEditor::RenderingPipelineEditor( RenderingPipeline& renderingPipeline )
: QMainWindow( NULL, 0 )
, m_renderingPipeline( renderingPipeline )
, m_graphWidget( NULL )
, m_docModified( false )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::initialize( TamyEditor& mgr )
{
   m_ui.setupUi( this );
   
   m_graphWidget = new GraphWidget( m_ui.mainEditorPanel );
   m_ui.mainEditorPanel->setWidget( m_graphWidget );
   connect( m_graphWidget, SIGNAL( getNodesClasses( std::vector< Class >& ) ), this, SLOT( onGetNodesClasses( std::vector< Class >& ) ) );
   connect( m_graphWidget, SIGNAL( getEdgesClasses( std::vector< Class >& ) ), this, SLOT( onGetEdgesClasses( std::vector< Class >& ) ) );
   connect( m_graphWidget, SIGNAL( addNode( void* ) ), this, SLOT( onAddNode( void* ) ) );
   connect( m_graphWidget, SIGNAL( removeNode( void* ) ), this, SLOT( onRemoveNode( void* ) ) );
   connect( m_graphWidget, SIGNAL( popupMenuShown( QMenu& ) ), this, SLOT( onPopupMenuShown( QMenu& ) ) );

   // configure the editor to display nodes in certain way
   m_graphWidget->associate< TestNode >( GraphBlock::GBS_RECTANGLE, QColor( 100, 200, 100 )  );

   show();
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::save()
{
   m_docModified = false;
   /// TODO: zapisac resource
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onGetNodesClasses( std::vector< Class >& classes )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< TestNode >( classes );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onGetEdgesClasses( std::vector< Class >& classes )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< TestEdge >( classes );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onAddNode( void* pNode )
{
   TestNode* node = reinterpret_cast< TestNode* >( pNode );
   m_renderingPipeline.addNode( node );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onRemoveNode( void* pNode )
{
   TestNode* node = reinterpret_cast< TestNode* >( pNode );
   m_renderingPipeline.removeNode( *node );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::onPopupMenuShown( QMenu& menu )
{
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineEditor::closeEvent( QCloseEvent *event )
{
   if ( m_docModified )
   {
      int choice = QMessageBox::question( this, tr("Save"), tr("Would you like to save your changes?"), QMessageBox::Yes | QMessageBox::No );
      if ( choice == QMessageBox::Yes )
      {
         save();
      }
   }

   // accept the event
   event->accept();

   delete this;
}

///////////////////////////////////////////////////////////////////////////////

// TODO:
// 1.) dokonczyc widget
// 2.) edycja parametrow node'a
