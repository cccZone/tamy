#pragma once

/// @file   TamyEditor\RenderingPipelineEditor.h
/// @brief  Editor allowing to edit a rendering pipeline resource

#include <QtGui/QMainWindow>
#include "ui_renderingpipelineeditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;
class GraphWidget;
class Class;
class GraphBlock;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineEditor : public QMainWindow, public ResourceEditor
{
   Q_OBJECT

private:
   Ui::RenderingPipelineEditorWindow         m_ui;

   RenderingPipeline&                        m_renderingPipeline;
   GraphWidget*                              m_graphWidget;
   bool                                      m_docModified;

public:
   /**
    * Constructor.
    *
    * @param renderingPipeline      pipeline to be edited
    */
   RenderingPipelineEditor( RenderingPipeline& renderingPipeline );

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

public slots:
   void save();
   void onGetNodesClasses( std::vector< Class >& classes );
   void onGetEdgesClasses( std::vector< Class >& classes );
   void onAddNode( void* pNode );
   void onRemoveNode( void* pNode );
   void onPopupMenuShown( QMenu& menu );

protected:
   void closeEvent( QCloseEvent *event );
};

///////////////////////////////////////////////////////////////////////////////
