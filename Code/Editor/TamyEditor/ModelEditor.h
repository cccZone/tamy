#pragma once

/// @file   TamyEditor\ModelEditor.h
/// @brief  scene tree editor operating on a scene itself

#include "SceneTreeEditor.h"
#include <string>
#include <QAction.h>
#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * Scene tree editor operating on a scene itself.
 */
class ModelEditor : public SceneTreeEditor
{
private:
   std::string          m_iconsDir;
   Model&               m_scene;

public:
   /**
    * Constructor.
    *
    * @param fs            file system containing the action's icon
    * @param scene   scene the editor will be modifying
    */
   ModelEditor( const Filesystem& fs, Model& scene );

   // -------------------------------------------------------------------------
   // SceneTreeEditor implementation
   // -------------------------------------------------------------------------
   void createAddEntityAction( QMenu& parent, const Class& entityClass );

   void createRemoveEntityAction( QMenu& parent, Entity* entity );

   void createClearAction( QMenu& parent );
};

///////////////////////////////////////////////////////////////////////////////

class MEAddEntityAction : public QAction
{
   Q_OBJECT

private:
   Model&   m_scene;
   Class    m_addedClass;

public:
   MEAddEntityAction( QWidget& parent, Model& scene, const Class& addedClass, const std::string& iconsDir );

public slots:
   void execute();
};

///////////////////////////////////////////////////////////////////////////////

class MERemoveEntityAction : public QAction
{
   Q_OBJECT

private:
   Model&   m_scene;
   Entity*  m_entityToRemove;

public:
   MERemoveEntityAction( QWidget& parent, Model& scene, Entity* entityToRemove, const std::string& iconsDir );

   public slots:
      void execute();
};

///////////////////////////////////////////////////////////////////////////////

class MEClearAction : public QAction
{
   Q_OBJECT

private:
   Model&   m_scene;

public:
   MEClearAction( QWidget& parent, Model& scene, const std::string& iconsDir );

public slots:
   void execute();
};

///////////////////////////////////////////////////////////////////////////////
