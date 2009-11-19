#pragma once

/// @file   TamyEditor\SceneQueries.h
/// @brief  component that allows to query scene for entities

#include "core\Component.h"
#include "core\SpatialStorage.h"
#include "QueryableScene.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for managing the selected entities.
 */
class SceneQueries : public Component<TamyEditor>
{
private:
   QueryableScene* m_entitiesQueryStorage;

public:
   /**
    * Constructor.
    */
   SceneQueries();
   ~SceneQueries();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);
};

///////////////////////////////////////////////////////////////////////////////
