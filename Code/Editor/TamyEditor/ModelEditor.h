#pragma once

/// @file   TamyEditor\ModelEditor.h
/// @brief  scene tree editor operating on a scene itself

#include "SceneTreeEditor.h"
#include <string>
#include "core\Class.h"


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
   void addEntity( const Class& entityClass );

   void removeEntity( Entity* entity );

   void clearEntity();
};

///////////////////////////////////////////////////////////////////////////////
