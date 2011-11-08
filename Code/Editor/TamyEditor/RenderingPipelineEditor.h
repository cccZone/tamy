#pragma once

/// @file   TamyEditor\RenderingPipelineEditor.h
/// @brief  Editor allowing to edit a rendering pipeline resource

#include <QtGui/QMainWindow>
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

   QVBoxLayout*                              m_nodePropertiesLayout;
   QPropertiesView*                          m_nodePropertiesRootView;

   QListWidget*                              m_renderTargetsList;

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
   void onDeinitialize();

public slots:
   void save();
   void onSceneSelectionChanged();
   void onShowContextMenu( const QPoint& pos );
   void addRenderTarget();
   void removeRenderTarget();
   void editRenderTarget( QListWidgetItem* rtItem );


private:
   void updateRenderTargetsList();
   void handleBlockSelection( GraphBlock* selectedBlock );
   void handleSocketSelection( GraphBlockSocket* selectedSocket );
};

///////////////////////////////////////////////////////////////////////////////

class RenderTargetsListWidget : public QListWidget
{
   Q_OBJECT

public:
   RenderTargetsListWidget( QWidget* parent );

protected:
   QMimeData* mimeData( const QList< QListWidgetItem *> items ) const;
};

///////////////////////////////////////////////////////////////////////////////
