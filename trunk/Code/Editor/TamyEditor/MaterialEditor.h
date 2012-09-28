/// @file   TamyEditor/MaterialEditor.h
/// @brief  Editor allowing to edit material resources
#pragma once

#include "ui_materialeditor.h"
#include "ResourceEditor.h"
#include <QListWidget>


///////////////////////////////////////////////////////////////////////////////

class MaterialLayout;
class GraphWidget;
class GraphBlock;
class GraphBlockSocket;
class QPropertiesView;

///////////////////////////////////////////////////////////////////////////////

class MaterialEditor : public ResourceEditor
{
   Q_OBJECT

private:
   Ui::MaterialEditorWindow                  m_ui;

   MaterialLayout&                           m_materialLayout;

   QVBoxLayout*                              m_blockPropertiesLayout;
   QPropertiesView*                          m_blockPropertiesRootView;

   QVBoxLayout*                              m_nodePropertiesLayout;
   QPropertiesView*                          m_nodePropertiesRootView;

public:
   /**
    * Constructor.
    *
    * @param materialLayout      material layout to be edited
    */
   MaterialEditor( MaterialLayout& materialLayout );
   ~MaterialEditor();

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
