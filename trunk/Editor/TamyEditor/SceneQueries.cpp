#include "SceneQueries.h"
#include "TamyEditor.h"
#include "core-Scene.h"
#include "core.h"
#include "EntitiesStorageView.h"
#include "QueryableSceneProxy.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueries::SceneQueries()
: m_entitiesQueryStorage(new QueryableSceneProxy())
, m_scene(NULL)
{
   associate<WorldEntity, EntitiesStorageView> ();
}

///////////////////////////////////////////////////////////////////////////////

SceneQueries::~SceneQueries()
{
   resetContents();

   delete m_entitiesQueryStorage; m_entitiesQueryStorage = NULL;
   m_scene = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::initialize(TamyEditor& mgr)
{
   m_scene = &(mgr.requestService<Model> ());
   m_scene->attach(*this);

   // register new service
   mgr.registerService<QueryableScene> (*m_entitiesQueryStorage);
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::setStorage(QueryableScene& storage)
{
   // filter will take care of deleting 'entitiesQueryView'
   NarrowPhaseStorageFilter<SpatiallyQueryable>* filteredStorage = 
      new NarrowPhaseStorageFilter<SpatiallyQueryable>(
         new SharedSpatialStorage<SpatiallyQueryable>(storage));

   m_entitiesQueryStorage->plugin(filteredStorage);
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::resetStorage(QueryableScene& storage)
{
   m_entitiesQueryStorage->plugin(NULL);
}

///////////////////////////////////////////////////////////////////////////////

Model& SceneQueries::scene()
{
   if (m_scene == NULL)
   {
      throw std::runtime_error("Scene is not initialized");
   }
   return *m_scene;
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::onEntityAdded(Entity& entity)
{
   Representations::iterator reprIt = m_representations.find(&entity);
   if (reprIt != m_representations.end())
   {
      // same entity instance can't have more than one representation
      return;
   }

   SceneQueriesModelRepresentation* representation = create(entity);
   if (representation != NULL)
   {
      representation->initialize(*this);
      m_representations.insert(std::make_pair(&entity, representation));
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::onEntityRemoved(Entity& entity)
{
   Representations::iterator it = m_representations.find(&entity);
   if (it != m_representations.end())
   {
      it->second->deinitialize(*this);
      delete it->second;
      m_representations.erase(it);
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::resetContents()
{
   m_entitiesQueryStorage->plugin(NULL);

   for (Representations::iterator it = m_representations.begin();
      it != m_representations.end(); ++it)
   {
      delete it->second;
   }
   m_representations.clear();
}

///////////////////////////////////////////////////////////////////////////////
