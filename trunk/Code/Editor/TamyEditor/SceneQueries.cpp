#include "SceneQueries.h"
#include "TamyEditor.h"
#include "core-MVC.h"
#include "core.h"
#include "core-Renderer.h"
#include "QueryableSceneProxy.h"

// representations
#include "QueryableGhost.h"
#include "QueryableSpatial.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueries::SceneQueries()
: m_entitiesQueryStorage(new QueryableSceneProxy())
{
   associate<Renderable, QueryableSpatial<Renderable> > ();
   associate<RenderableJoint, QueryableGhost<RenderableJoint> > ();
}

///////////////////////////////////////////////////////////////////////////////

SceneQueries::~SceneQueries()
{
   resetContents();

   delete m_entitiesQueryStorage; m_entitiesQueryStorage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::initialize(TamyEditor& mgr)
{
   Model& scene = mgr.requestService<Model> ();
   scene.attach(*this);

   // register new service
   mgr.registerService<QueryableScene> (*m_entitiesQueryStorage);
}

///////////////////////////////////////////////////////////////////////////////

QueryableScene& SceneQueries::storage()
{
   return *m_entitiesQueryStorage;
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
      SceneQueriesModelRepresentation* repr = it->second;
      m_representations.erase(it);

      repr->deinitialize(*this);
      delete repr;
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
