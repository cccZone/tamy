#include "QueryableWorld.h"
#include "SceneQueries.h"
#include "SpatiallyQueryable.h"
#include "core-Scene\WorldEntity.h"
#include "core-Scene\Model.h"
#include "core.h"
#include <stdexcept>
#include "QueryableScene.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

class QueryableSceneImpl : public QueryableScene
{
private:
   RegularOctree<SpatiallyQueryable>* m_storage;
   SpatialStorage<SpatiallyQueryable>* m_queryStorage;

public:
   QueryableSceneImpl(const AABoundingBox& worldSize)
   : m_storage(NULL)
   , m_queryStorage(NULL)
   {
      int maxElemsPerSector = 64;
      int maxTreeDepth = 5;
      m_storage = new RegularOctree<SpatiallyQueryable> (worldSize, 
         maxElemsPerSector,
         maxTreeDepth);

      // filter will take care of deleting 'entitiesQueryView'
      m_queryStorage = new NarrowPhaseStorageFilter<SpatiallyQueryable>(m_storage);
   }

   ~QueryableSceneImpl()
   {
      m_storage = NULL;
      delete m_queryStorage; m_queryStorage = NULL;
   }

   // -------------------------------------------------------------------------
   // QueryableScene implementation
   // -------------------------------------------------------------------------
   void query(const BoundingVolume& boundingVol, 
              Array<SpatiallyQueryable*>& output) const
   {
      m_queryStorage->query(boundingVol, output);
   }

   void insert(SpatiallyQueryable& entity)
   {
      m_storage->insert(entity);
   }

   void remove(SpatiallyQueryable& entity)
   {
      m_storage->remove(entity);
   }
};

} // anonymous

///////////////////////////////////////////////////////////////////////////////

QueryableWorld::QueryableWorld(WorldEntity& entity)
: m_entity(entity)
{
}

///////////////////////////////////////////////////////////////////////////////

void QueryableWorld::initialize(SceneQueries& parent)
{
   parent.setStorage(new QueryableSceneImpl(m_entity.m_size));
}

///////////////////////////////////////////////////////////////////////////////

void QueryableWorld::deinitialize(SceneQueries& parent)
{
   parent.setStorage(NULL);
}

///////////////////////////////////////////////////////////////////////////////
