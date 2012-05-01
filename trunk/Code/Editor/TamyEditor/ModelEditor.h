/// @file   TamyEditor\ModelEditor.h
/// @brief  scene tree editor operating on a scene itself
#pragma once

#include "SceneTreeEditor.h"
#include <string>
#include "core\ReflectionType.h"


///////////////////////////////////////////////////////////////////////////////

class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * Scene tree editor operating on a scene itself.
 */
class ModelEditor : public SceneTreeEditor
{
private:
   Model&               m_scene;

public:
   /**
    * Constructor.
    *
    * @param scene   scene the editor will be modifying
    */
   ModelEditor( Model& scene );

   // -------------------------------------------------------------------------
   // SceneTreeEditor implementation
   // -------------------------------------------------------------------------
   void addEntity( const SerializableReflectionType& entityClass );

   void removeEntity( Entity* entity );

   void clearEntity();
};

///////////////////////////////////////////////////////////////////////////////
