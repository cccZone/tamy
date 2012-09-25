/// @file   TamyEditor\RenderingPipelineEditor.h
/// @brief  Editor allowing to edit rendering pipeline resources
#pragma once

#include "ui_renderingpipelineeditor.h"
#include "ResourceEditor.h"
#include <QListWidget>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineLayout;
class GraphWidget;
class Class;
class GraphBlock;
class GraphBlockSocket;
class QPropertiesView;
class Object;

///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::RenderingPipelineEditorWindow         m_ui;

   RenderingPipelineLayout&                  m_renderingPipelineLayout;

   QVBoxLayout*                              m_blockPropertiesLayout;
   QPropertiesView*                          m_blockPropertiesRootView;

   QListWidget*                              m_renderTargetsList;
   QListWidget*                              m_depthBuffersList;

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
   void onInitialize();
   void onDeinitialize( bool saveProgress );

public slots:
   void save();
   void onSceneSelectionChanged();
   void onShowContextMenu( const QPoint& pos );
   void addRenderTarget();
   void removeRenderTarget();
   void editRenderTarget( QListWidgetItem* rtItem );
   void addDepthBuffer();
   void removeDepthBuffer();
   void editDepthBuffer( QListWidgetItem* dbItem );
   void onBlockAdded();
   void onBlockRemoved();

private:
   void updateRenderTargetsList();
   void updateDepthBuffersList();
   void handleBlockSelection( GraphBlock* selectedBlock );
};

///////////////////////////////////////////////////////////////////////////////
