#pragma once

/// @file   TamyEditor\PropertiesEditor.h
/// @brief  component for editing entity properties

#include "core\Component.h"
#include "core-Scene\PropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Entity;
class QMainWindow;
class QMenu;
class QLayout;
class QWidget;
class QSpacerItem;
class QLabel;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for navigating scene.
 */
class PropertiesEditor : public Component<TamyEditor>, 
                         public TPropertiesView<PropertiesEditor>
{
private:
   QLayout* m_propertiesLayout;
   QSpacerItem* m_spacer;
   QLabel* m_entityClassName;

public:
   /**
    * Constructor.
    */
   PropertiesEditor();
   ~PropertiesEditor();

   // -------------------------------------------------------------------------
   // Editor widgets management
   // -------------------------------------------------------------------------
   /**
    * This method adds a new single-property editor widget to the properties
    * editor window.
    *
    * @param editorWidget  widget of the editor we want to add
    */
   void addPropertyEditor(QWidget* editorWidget);

   /**
    * This method removes a single-property editor widget from the properties
    * editor window.
    *
    * @param editorWidget  widget of the editor we want to remove
    */
   void removePropertyEditor(QWidget& editorWidget);

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);

   // -------------------------------------------------------------------------
   // Selected entities management
   // -------------------------------------------------------------------------
   /**
    * This method allows to select an entity for edition.
    *
    * @param entity  entity properties of which we want to edit.
    */
   void selectEntity(Entity& entity);

   /**
    * Removes the selected entity's properties from the view.
    */
   void resetSelection();

private:

   void initUI(QMainWindow& mainWindow, QMenu& viewMenu);
};

///////////////////////////////////////////////////////////////////////////////
