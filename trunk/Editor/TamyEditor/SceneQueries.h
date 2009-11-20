#pragma once

/// @file   TamyEditor\SceneQueries.h
/// @brief  component that allows to query scene for entities

#include "core\Component.h"
#include "core\GenericFactory.h"
#include "core-Scene\ModelView.h"
#include "QueryableScene.h"


///////////////////////////////////////////////////////////////////////////////

class TamyEditor;
class SceneQueriesModelRepresentation;
class Entity;
class Model;
class QueryableSceneProxy;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component is responsible for managing the selected entities.
 */
class SceneQueries : public Component<TamyEditor>,
                     public GenericFactory<Entity, SceneQueriesModelRepresentation>,
                     public ModelView
{
private:
   QueryableSceneProxy* m_entitiesQueryStorage;

   typedef std::map<Entity*, SceneQueriesModelRepresentation*> Representations;
   Representations m_representations;

   Model* m_scene;

public:
   /**
    * Constructor.
    */
   SceneQueries();
   ~SceneQueries();

   // -------------------------------------------------------------------------
   // Storage management
   // -------------------------------------------------------------------------
   /**
    * Sets a new instance of the storage which will contain queryable entities.
    *
    * @param storage    storage with queryable scene entities
    */
   void setStorage(QueryableScene& storage);

   /**
    * Removes currently used queryable entities storage.
    *
    * @param storage    storage with queryable scene entities
    */
   void resetStorage(QueryableScene& storage);

   /**
    * Returns the instance of the scene this instance is working with.
    *
    * @return           scene model
    */
   Model& scene();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize(TamyEditor& mgr);

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded(Entity& entity);

   void onEntityRemoved(Entity& entity);

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
