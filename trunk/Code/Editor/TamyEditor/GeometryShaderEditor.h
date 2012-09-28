/// @file   TamyEditor/GeometryShaderEditor.h
/// @brief  Editor allowing to edit geometry shader resources
#pragma once

#include "ui_geometryshadereditor.h"
#include "ResourceEditor.h"
#include <QListWidget>


///////////////////////////////////////////////////////////////////////////////

class GeometryShaderLayout;
class GraphWidget;
class GraphBlock;
class GraphBlockSocket;
class QPropertiesView;

///////////////////////////////////////////////////////////////////////////////

class GeometryShaderEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::GeometryShaderEditorWindow            m_ui;

   GeometryShaderLayout&                     m_geometryShaderLayout;

   QVBoxLayout*                              m_blockPropertiesLayout;
   QPropertiesView*                          m_blockPropertiesRootView;

   QVBoxLayout*                              m_nodePropertiesLayout;
   QPropertiesView*                          m_nodePropertiesRootView;

public:
   /**
    * Constructor.
    *
    * @param geometryShaderLayout      geometry shader layout to be edited
    */
   GeometryShaderEditor( GeometryShaderLayout& geometryShaderLayout );
   ~GeometryShaderEditor();

   // -------------------------------------------------------------------------
   // ResourceEditor implementation
   // -------------------------------------------------------------------------
   void onInitialize();
   void onDeinitialize( bool saveProgress );

public slots:
   void save();
   void onSceneSelectionChanged();
   void onShowContextMenu( const QPoint& pos );

private:
   void handleBlockSelection( GraphBlock* selectedBlock );
};

///////////////////////////////////////////////////////////////////////////////
