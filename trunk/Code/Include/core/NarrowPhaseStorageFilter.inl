#ifndef _NARROW_PHASE_STORAGE_FILTER_H
#error "This file can only be included from NarrowPhaseStorageFilter.h"
#else

#include <stdexcept>
#include "core\Triangle.h"


///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
NarrowPhaseStorageFilter<Elem>::NarrowPhaseStorageFilter(SpatialStorage<Elem>* storage)
: m_storage(storage)
{
   if (m_storage == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a SpatialStorage instance");
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
NarrowPhaseStorageFilter<Elem>::~NarrowPhaseStorageFilter()
{
   delete m_storage; m_storage = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Elem>
void NarrowPhaseStorageFilter< Elem >::query( const BoundingVolume& boundingVol, Array<Elem*>& output ) const
{
   Array< Elem* > possiblyVisibleNodes;
   m_storage->query( boundingVol, possiblyVisibleNodes );

   unsigned int nodesCount = possiblyVisibleNodes.size();
   if ( nodesCount == 0 ) 
   {
      return;
   }

   BoundingVolume* transformedVolume = boundingVol.clone();

   // perform a narrow phase query
   unsigned int trisCount;
   const Array< Triangle* >* geometry;

   Elem* node = NULL;
   bool visible = false;
   Matrix volumeTransformMtx;
   for ( unsigned int i = 0; i < nodesCount; ++i )
   {
      visible = false;
      node = possiblyVisibleNodes[i];

      geometry = &node->getBoundingGeometry();
      trisCount = geometry->size();
      if (trisCount == 0)
      {
         visible = true;
      }
      else
      {
         volumeTransformMtx.setInverse( node->getGlobalMtx() );
         for (unsigned int j = 0; j < trisCount; ++j)
         {
            boundingVol.transform( volumeTransformMtx, *transformedVolume );
            Triangle* tri = ( *geometry )[j];
            if ( tri->testCollision( *transformedVolume ) == true ) 
            {
               visible = true;
               break;
            }
         }
      }

      if (visible)
      {
         output.push_back(node);
      }
   }

   // cleanup
   delete transformedVolume;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _NARROW_PHASE_STORAGE_FILTER_H