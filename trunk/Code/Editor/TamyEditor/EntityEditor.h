#pragma once

/// @file   TamyEditor\EntityEditor.h
/// @brief  scene tree editor operating on the specified entity

#include "SceneTreeEditor.h"
#include <string>
#include <QAction.h>
#include "core\Class.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;

///////////////////////////////////////////////////////////////////////////////

/**
 * Scene tree editor operating on the specified entity.
 */
class EntityEditor : public SceneTreeEditor
{
private:
   std::string          m_iconsDir;
   Entity&              m_editedEntity;

public:
   /**
    * Constructor.
    *
    * @param fs             file system containing the action's icon
    * @param editedEntity   entity the editor will be modifying
    */
   EntityEditor( const Filesystem& fs, Entity& editedEntity );

   // -------------------------------------------------------------------------
   // SceneTreeEditor implementation
   // -------------------------------------------------------------------------
   void createAddEntityAction( QMenu& parent, const Class& entityClass );

   void createRemoveEntityAction( QMenu& parent, Entity* entity );

   void createClearAction( QMenu& parent );
};

///////////////////////////////////////////////////////////////////////////////

class EEAddEntityAction : public QAction
{
   Q_OBJECT

private:
   Entity&  m_parentEntity;
   Class    m_addedClass;

public:
   EEAddEntityAction( QWidget& parent, Entity& parentEntity, const Class& addedClass, const std::string& iconsDir );

public slots:
   void execute();
};

///////////////////////////////////////////////////////////////////////////////

class EERemoveEntityAction : public QAction
{
   Q_OBJECT

private:
   Entity&  m_editedEntity;
   Entity*  m_entityToRemove;

public:
   EERemoveEntityAction( QWidget& parent, Entity& editedEntity, Entity* entityToRemove, const std::string& iconsDir );

public slots:
   void execute();
};

///////////////////////////////////////////////////////////////////////////////

class EEClearAction : public QAction
{
   Q_OBJECT

private:
   Entity&  m_editedEntity;

public:
   EEClearAction( QWidget& parent, Entity& editedEntity, const std::string& iconsDir );

public slots:
   void execute();
};

///////////////////////////////////////////////////////////////////////////////
