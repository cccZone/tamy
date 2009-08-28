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
 * An octree representation.
 */
template<typename Elem>
class Octree : public SpatialStorage<Elem>
{
private:
   /**
    * A helper structure that describes a single tree node.
    */
   struct Sector
   {
      AABoundingBox m_bb;
      Sector** m_children;
      Array<unsigned int> m_elems;

      Sector();

      ~Sector();
   };


private:
   Array<Elem*> m_elements;

   unsigned int m_maxElemsPerSector;
   Sector* m_root;

public:
   Octree(unsigned int maxElemsPerSector = 64, float worldSize = 1000);

   ~Octree();

   /**
    * The method checks if an element is present in the tree.
    *
    * @param elem    element presence of which we want to check
    * @return        'true' if element was successfully added,
    *                'false' otherwise
    */
   bool isAdded(const Elem& elem) const;

   /**
    * The method inserts a new element into the tree.
    *
    * @param elem    element we want to add
    */
   void insert(Elem& elem);

   /**
    * The method allows to remove an element from a tree.
    *
    * @param elem    element we want to remove
    */
   void remove(Elem& elem);

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

private:
   void querySectors(const BoundingVolume& boundingVol, 
                     Sector& searchRoot,
                     Array<Sector*>& output) const;

   void subdivideSector(Sector& sector);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Octree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _OCTREE_H
