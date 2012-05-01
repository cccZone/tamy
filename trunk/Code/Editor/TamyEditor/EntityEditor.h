#pragma once

/// @file   TamyEditor\EntityEditor.h
/// @brief  scene tree editor operating on the specified entity

#include "SceneTreeEditor.h"
#include <string>
#include "core\ReflectionType.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Scene tree editor operating on the specified entity.
 */
class EntityEditor : public SceneTreeEditor
{
private:
   Entity&              m_editedEntity;

public:
   /**
    * Constructor.
    *
    * @param editedEntity   entity the editor will be modifying
    */
   EntityEditor( Entity& editedEntity );

   // -------------------------------------------------------------------------
   // SceneTreeEditor implementation
   // -------------------------------------------------------------------------
   void addEntity( const SerializableReflectionType& entityClass );

   void removeEntity( Entity* entity );

   void clearEntity();
};

///////////////////////////////////////////////////////////////////////////////
