#ifndef _OCTREE_H
#define _OCTREE_H

/// @file   core\Octree.h
/// @brief  an octree spatial container

#include "core\SpatialStorage.h"
#include "core\Array.h"
#include "core\Stack.h"
#include "core\AABoundingBox.h"
#include "core\BoundingVolume.h"
#include "core\Assert.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * An octree representation. This is an abstract version of such a tree.
 * Each tree inherited from it will be queried in the same way, however
 * the way it is constructed depends on particular implementations.
 */
template<typename Elem>
class Octree : public SpatialStorage<Elem>
{
protected:
   enum ChildSectors
   {
      CS_FRONT_RIGHT_UPPER,
      CS_FRONT_RIGHT_LOWER,
      CS_FRONT_LEFT_UPPER,
      CS_FRONT_LEFT_LOWER,
      CS_BACK_RIGHT_UPPER,
      CS_BACK_RIGHT_LOWER,
      CS_BACK_LEFT_UPPER,
      CS_BACK_LEFT_LOWER,
   };

   /**
    * A helper structure that describes a single tree node.
    */
   struct Sector
   {
      unsigned int m_depth;
      AABoundingBox m_bb;
      Sector** m_children;
      Array<unsigned int> m_elems;

      Sector();

      ~Sector();
   };


protected:
   // root node of the tree
   Sector* m_root;

public:
   /**
    * Constructor. 
    *
    * @param treeBB     bounding box for the tree
    */
   Octree(const AABoundingBox& treeBB);

   virtual ~Octree();

   /**
    * This method allows to query all the elements that are contained
    * in the passed bounding volume.
    *
    * What is important here is that a function:
    *    bool testCollision(const AABoundingBox& bb, const BoundingVolumeType& bv)
    * is made available to the linker.
    *
    * @param boundingVol   volume that bounds the elements we want
    * @param output        upon method return this array will be filled with
    *                      elements overlapping the query volume
    */
   void query(const BoundingVolume& boundingVol, Array<Elem*>& output) const;

   /**
    * This utility method allows to subdivide the specified sector
    * so that it becomes an octree itself.
    *
    * @param sector     sector we want to divide.
    */
   void subdivideSector(Sector& sector);

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
   void querySectors(const BoundingVolume& boundingVol, 
                     Sector& searchRoot,
                     Array<Sector*>& output) const;

protected:
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

   /**
    * Deletes all sectors.
    */
   void clearSectors();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Octree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _OCTREE_H
