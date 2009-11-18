#include "EntitiesStorageView.h"
#include "core-Scene\SpatiallyQueryable.h"
#include "core-Scene\Entity.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

EntitiesStorageView::EntitiesStorageView(const AABoundingBox& treeBB, 
                                         unsigned int maxElemsPerSector,
                                         unsigned int maxTreeDepth)
: m_storage(new RegularOctree<SpatiallyQueryable> (treeBB, maxElemsPerSector, maxTreeDepth))
{
   if (m_storage == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a SpatialStorage instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

EntitiesStorageView::~EntitiesStorageView()
{
   delete m_storage; m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::query(const BoundingVolume& boundingVol, 
                                Array<SpatiallyQueryable*>& output) const
{
   m_storage->query(boundingVol, output);
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::onEntityAdded(Entity& entity)
{
   SpatiallyQueryable* queryable = dynamic_cast<SpatiallyQueryable*> (&entity);
   if (queryable == NULL) 
   {
      return;
   }

   m_storage->insert(*queryable);
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::onEntityRemoved(Entity& entity)
{
   SpatiallyQueryable* queryable = dynamic_cast<SpatiallyQueryable*> (&entity);
   if (queryable == NULL) 
   {
      return;
   }

   m_storage->remove(*queryable);
}

///////////////////////////////////////////////////////////////////////////////

void EntitiesStorageView::resetContents()
{
   while (m_storage->getElementsCount())
   {
      SpatiallyQueryable* queryable = &(m_storage->getElement(0));
      m_storage->remove(*queryable);
   }
}

///////////////////////////////////////////////////////////////////////////////
