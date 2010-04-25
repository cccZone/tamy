#pragma once

/// @file   TamyEditor\PropertiesEditor.h
/// @brief  component for editing entity properties

#include "core\Component.h"
#include "core\PropertiesView.h"
#include "QPropertyEditor.h"

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
 * This component is responsible for navigating scene.
 */
class PropertiesEditor : public Component<TamyEditor>
{
private:
   QVBoxLayout*      m_layout;
   QPropertiesView*  m_rootView;

   Effect*           m_selectionMarker;

   Entity*           m_selectedEntity;

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

   // -------------------------------------------------------------------------
   // Selected entities management
   // -------------------------------------------------------------------------
   /**
    * This method allows to select an object for edition.
    *
    * @param object  object properties of which we want to edit.
    */
   void selectObject(Object& object);

   /**
    * Removes the selected object's properties from the view.
    */
   void resetSelection();

private:
   void initUI(QMainWindow& mainWindow, QMenu& viewMenu);
   void visualizeSelection( Entity* newSelection );
};

///////////////////////////////////////////////////////////////////////////////
