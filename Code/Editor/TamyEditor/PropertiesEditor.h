#pragma once

/// @file   TamyEditor\PropertiesEditor.h
/// @brief  component for editing entity properties

#include "core\Component.h"
#include "core\PropertiesView.h"
#include "QPropertyEditor.h"
#include "SelectionManager.h"

///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Object;
class QPropertiesView;
class QMainWindow;
class QMenu;
class Effect;
class Entity;

///////////////////////////////////////////////////////////////////////////////

/**
 * Component for editing entity properties.
 */
class PropertiesEditor : public Component<TamyEditor>, public SelectionManagerListener
{
private:
   QVBoxLayout*      m_layout;
   QPropertiesView*  m_rootView;

   SelectionManager* m_selectionManager;

public:
   /**
    * Constructor.
    */
   PropertiesEditor();
   ~PropertiesEditor();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);
   void onServiceRegistered( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // SelectionManagerListener implementation
   // -------------------------------------------------------------------------
   void onObjectSelected( Entity& entity );
   void onObjectDeselected( Entity& entity );

private:
   void initUI(QMainWindow& mainWindow, QMenu& viewMenu);
};

///////////////////////////////////////////////////////////////////////////////
