#pragma once

/// @file   TamyEditor\SceneQueries.h
/// @brief  component that allows to query scene for entities

#include "core\Component.h"
#include "core\GenericFactory.h"
#include "core-MVC\ModelView.h"
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
   typedef std::map<Entity*, SceneQueriesModelRepresentation*> Representations;

private:
   QueryableSceneProxy* m_entitiesQueryStorage;
   Representations      m_representations;
   Model*               m_currObservedScene;

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
   void setStorage(QueryableScene* storage);

   /**
    * Gives access to the storage that contains all queryable entity
    * representations.
    *
    * @return      queryable scene
    */
   QueryableScene& storage();

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( TamyEditor& mgr );
   void onServiceRegistered( TamyEditor& mgr );

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded( Entity& entity );
   void onEntityRemoved( Entity& entity );
   void onEntityChanged( Entity& entity );

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
