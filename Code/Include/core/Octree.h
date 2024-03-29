/// @file   core\Octree.h
/// @brief  an octree spatial container
#ifndef _OCTREE_H
#define _OCTREE_H

#include "core\SpatialStorage.h"
#include "core\OctreeNode.h"
#include "core\Stack.h"
#include "core\AABoundingBox.h"
#include "core\BoundingVolume.h"
#include "core\Assert.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class MemoryPool;
class MemoryPoolAllocator;

///////////////////////////////////////////////////////////////////////////////

/**
 * An octree representation. This is an abstract version of such a tree.
 * Each tree inherited from it will be queried in the same way, however
 * the way it is constructed depends on particular implementations.
 */
template<typename Elem>
class Octree : public SpatialStorage<Elem>
{
   DECLARE_ALLOCATOR( Octree< Elem >, AM_ALIGNED_16 );

protected:
   typedef OctreeNode<unsigned int> Sector;

   // root node of the tree
   Sector* m_root;

   // allocator
   MemoryPool*             m_memoryPool;
   MemoryPoolAllocator*    m_allocator;

public:
   /**
    * Constructor. 
    *
    * @param treeBB     bounding box for the tree
    */
   Octree( const AABoundingBox& treeBB );
   virtual ~Octree();

   /**
    * This utility method will subdivide the specified subtree
    * to the specified depth. The depth is relative to
    * the subtree root, so if we pass a sector at depth 4 i.e.,
    * and specify the depth as 2, the subtree's gonna get subdivided
    * up to level 6.
    *
    * @param root    root of the subtree we wish  to subdivide
    * @param depth   relative depth of partitioning
    */
   void subdivideTree(Sector& root, unsigned int depth);

   /**
    * The method allows to query for sectors that overlap
    * the specified bounding volume.
    *
    * @param boundingVol   volume we want to use for the overlap test
    * @param searchRoot    root of a subtree we want to search in
    * @param output        upon method return this array will be filled
    *                      with found sectors that match the query criteria.
    */
   void querySectors( const BoundingVolume& boundingVol, Sector& searchRoot, Array< Sector*, MemoryPoolAllocator >& output ) const;

   /**
    * Calculates and returns actual boundaries of the scene stored in the storage.
    *
    * @param outBounds
    */
   void getSceneBounds( AABoundingBox& outBounds ) const;

   // -------------------------------------------------------------------------
   // SpatialStorage implementation
   // -------------------------------------------------------------------------
   void query( const BoundingVolume& boundingVol, Array<Elem*>& output ) const;

   /**
    * This method returns a total number of elements stored in the tree.
    * 
    * @return     tree elements count
    */
   virtual unsigned int getElementsCount() const = 0;

   /**
    * The method returns an element specified with the index.
    *
    * @param idx  index of the element in the tree
    * @return     element corresponding to the specified index
    */
   virtual Elem& getElement(unsigned int idx) const = 0;

protected:

   /**
    * Recalculates the hierarchical boundaries of stored elements.
    */
   void recalculateElementsBounds();

   /**
    * Deletes all sectors.
    */
   void clearSectors();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Octree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _OCTREE_H
