#pragma once

/// @file   TamyEditor\RenderingPipelineEditor.h
/// @brief  Editor allowing to edit a rendering pipeline resource

#include <QtGui/QMainWindow>
#include "ui_renderingpipelineeditor.h"
#include "ResourceEditor.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineLayout;
class GraphWidget;
class Class;
class GraphBlock;
class QPropertiesView;
class Object;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineEditor : public QMainWindow, public ResourceEditor
{
   Q_OBJECT

private:
   Ui::RenderingPipelineEditorWindow         m_ui;

   RenderingPipelineLayout&                  m_renderingPipelineLayout;
   QVBoxLayout*                              m_propertiesLayout;
   QPropertiesView*                          m_rootView;

public:
   /**
    * Constructor.
    *
    * @param renderingPipeline      pipeline to be edited
    */
   RenderingPipelineEditor( RenderingPipelineLayout& renderingPipelineLayout );
   ~RenderingPipelineEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );

public slots:
   void save();
   void onSceneSelectionChanged();
   void onShowContextMenu( const QPoint& pos );

protected:
   void closeEvent( QCloseEvent *event );
};

///////////////////////////////////////////////////////////////////////////////
