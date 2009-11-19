#include "SceneQueries.h"
#include "TamyEditor.h"
#include "core-Scene.h"
#include "core.h"
#include "EntitiesStorageView.h"


///////////////////////////////////////////////////////////////////////////////

SceneQueries::SceneQueries()
: m_entitiesQueryStorage(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

SceneQueries::~SceneQueries()
{
   delete m_entitiesQueryStorage; m_entitiesQueryStorage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneQueries::initialize(TamyEditor& mgr)
{
   Model& scene = mgr.requestService<Model> ();

   // set up the queryable scene storage
   AABoundingBox sceneBB(D3DXVECTOR3(-2000, -2000, -2000), D3DXVECTOR3(2000, 2000, 2000));
   EntitiesStorageView* entitiesQueryView = new EntitiesStorageView(sceneBB);
   scene.attach(*entitiesQueryView);

   // filter will take care of deleting 'entitiesQueryView'
   NarrowPhaseStorageFilter<SpatiallyQueryable>* filteredStorage = 
      new NarrowPhaseStorageFilter<SpatiallyQueryable>(entitiesQueryView);
   m_entitiesQueryStorage = filteredStorage;

   // register new service
   mgr.registerService<QueryableScene> (*m_entitiesQueryStorage);
}

///////////////////////////////////////////////////////////////////////////////
