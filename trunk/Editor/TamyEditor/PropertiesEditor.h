#pragma once

/// @file   TamyEditor\PropertiesEditor.h
/// @brief  component for editing entity properties

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class Entity;
class QMainWindow;
class QMenu;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for navigating scene.
 */
class PropertiesEditor : public Component<TamyEditor>
{
private:
   bool m_isUIVisible;

public:
   /**
    * Constructor.
    */
   PropertiesEditor();
   ~PropertiesEditor();

   /**
    * This method allows to select an entity for edition.
    *
    * @param entity  entity properties of which we want to edit.
    */
   void selectEntity(Entity& entity);

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);

private:
   void initUI(QMainWindow& mainWindow, QMenu& viewMenu);
};

///////////////////////////////////////////////////////////////////////////////
