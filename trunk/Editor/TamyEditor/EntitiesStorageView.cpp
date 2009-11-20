#include "EntitiesStorageView.h"
#include "SceneQueries.h"
#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\WorldEntity.h"
#include "core-Scene\Model.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

EntitiesStorageView::EntitiesStorageView(WorldEntity& entity)
: m_entity(entity)
, m_storage(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

EntitiesStorageView::~EntitiesStorageView()
{
   delete m_storage; m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::initialize(SceneQueries& parent)
{
   int maxElemsPerSector = 64;
   int maxTreeDepth = 5;
   m_storage = new RegularOctree<SpatiallyQueryable> (m_entity.m_size, 
                                                      maxElemsPerSector,
                                                      maxTreeDepth);

   parent.setStorage(*this);
   parent.scene().attach(*this);
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::deinitialize(SceneQueries& parent)
{
   parent.scene().detach(*this);
   parent.resetStorage(*this);
   delete m_storage; m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::query(const BoundingVolume& boundingVol, 
                                Array<SpatiallyQueryable*>& output) const
{
   if (m_storage != NULL)
   {
      m_storage->query(boundingVol, output);
   }
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::onEntityAdded(Entity& entity)
{
   SpatiallyQueryable* queryable = dynamic_cast<SpatiallyQueryable*> (&entity);
   if ((queryable == NULL) || (m_storage == NULL))
   {
      return;
   }

   m_storage->insert(*queryable);
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::onEntityRemoved(Entity& entity)
{
   SpatiallyQueryable* queryable = dynamic_cast<SpatiallyQueryable*> (&entity);
   if ((queryable == NULL) || (m_storage == NULL)) 
   {
      return;
   }

   m_storage->remove(*queryable);
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::resetContents()
{
   if (m_storage != NULL)
   {
      m_storage->clear();
   }
}

///////////////////////////////////////////////////////////////////////////////
