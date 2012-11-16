#ifndef _OCTREE_H
#error "This file can only be included from Octree.h"
#else

#include "core\MemoryPoolAllocator.h"
#include "core\List.h"


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::Octree( const AABoundingBox& treeBB )
   : m_root( new Sector( treeBB ) )
{
   m_memoryPool = new MemoryPool( 65535 );
   m_allocator = new MemoryPoolAllocator( m_memoryPool );
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
Octree<Elem>::~Octree()
{
   delete m_root;
   m_root = NULL;

   delete m_allocator;
   m_allocator = NULL;

   delete m_memoryPool;
   m_memoryPool = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Elem >
void Octree< Elem >::query( const BoundingVolume& boundingVol, Array< Elem* >& output ) const
{
   Array< Sector*, MemoryPoolAllocator > candidateSectors( 16, m_allocator );
   querySectors( boundingVol, *m_root, candidateSectors );

   unsigned int elemsCount = getElementsCount();
   Array< bool, MemoryPoolAllocator > elemsToOutput( 16, m_allocator );
   elemsToOutput.resize( elemsCount, false );

   unsigned int sectorsCount = candidateSectors.size();
   Array< unsigned int >* elemsList = NULL;
   unsigned int sectorElemsCount = 0;
   for ( unsigned int i = 0; i < sectorsCount; ++i )
   {
      elemsList = ( &candidateSectors[i]->m_elems );
      sectorElemsCount = elemsList->size();
      for ( unsigned int j = 0; j < sectorElemsCount; ++j )
      {
         int elemIdx = ( *elemsList )[j];
         elemsToOutput[elemIdx] = true;
      }
   }

   // add each element only once - providing it's inside the query volume
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {
      if ( elemsToOutput[i] == false ) 
      {
         continue;
      }
      
      Elem& elem = getElement(i);
      if ( elem.getBoundingVolume().testCollision( boundingVol ) )
      {
         output.push_back( &elem );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Elem >
void Octree< Elem >::querySectors( const BoundingVolume& boundingVol, Sector& searchRoot, Array< Sector*, MemoryPoolAllocator >& output ) const
{
   Stack< Sector*, MemoryPoolAllocator > stack( m_allocator );
   stack.push( &searchRoot );

   while( stack.empty() == false )
   {
      Sector* currSector = stack.pop();

      if (currSector->doesIntersect( boundingVol ) == false) {continue;}

      unsigned int childrenCount = currSector->getChildrenCount();
      if ( childrenCount > 0 )
      {
         ASSERT_MSG( currSector->m_elems.size() == 0, "Composite node has an element assigned" );
         for ( unsigned int i = 0; i < childrenCount; ++i )
         {
            stack.push( &currSector->getChild(i) );
         }
      }
      else
      {
         output.push_back( currSector );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::subdivideTree(Sector& root, unsigned int depth)
{
   unsigned int desiredDepth = root.getDepth() + depth;

   List< Sector*, MemoryPoolAllocator > sectorsQueue( m_allocator );
   sectorsQueue.pushBack(m_root);
   
   while( !sectorsQueue.empty() )
   {
      Sector* sector = sectorsQueue.front();
      
      if (sector->getDepth() < desiredDepth)
      {
         sector->subdivide();

         unsigned int childrenCount = sector->getChildrenCount();
         for ( unsigned int i = 0; i < childrenCount; ++i )
         {
            sectorsQueue.pushBack( &sector->getChild(i) );
         }
      }

      sectorsQueue.popFront();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::clearSectors()
{
   AABoundingBox treeBB = m_root->getBoundingBox();
   unsigned int depth = m_root->getDepth();
   delete m_root;
   m_root = new Sector( treeBB, depth );
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::getSceneBounds( AABoundingBox& outBounds ) const
{
   outBounds = m_root->m_globalElementsBounds;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void Octree<Elem>::recalculateElementsBounds()
{
   m_root->recalculateGlobalBounds( *this );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _OCTREE_H
