#ifndef _DYNAMIC_OCTREE_H
#define _DYNAMIC_OCTREE_H

/// @file   core\DynamicOctree.h
/// @brief  an octree capable of storing dynamic (moving) objects

#include "core\SpatialStorage.h"
#include "core\Array.h"
#include "core\Stack.h"
#include "core\AABoundingBox.h"
#include "core\BoundingVolume.h"
#include "core\Assert.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * An octree that can store dynamic objects and regenerates itself
 * whenever the objects change their location.
 */
template<typename Elem>
class DynamicOctree : public SpatialStorage<Elem>
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

   /**
    * A helper structure for storing elements added to the tree.
    */
   struct TreeElem
   {
      Elem* elem;
      Array<Sector*> hostSectors;

      TreeElem(Elem* _elem);
   };


private:
   Array<TreeElem*> m_elements;
   Sector* m_root;

public:
   /**
    * Constructor. 
    * The tree is precreated here - no further subdivision
    * will happen after the tree's been created.
    *
    * @param treeBB     bounding box for the tree
    * @param treeDepth  depth at which we want the tree to have its leaves.
    *                   Depth == 0 is allowed and means that the tree has
    *                   only one node where everything is sotred - thus it
    *                   becomes nothing more than a mere linear storage.
    */
   DynamicOctree(const AABoundingBox& treeBB, int treeDepth);

   ~DynamicOctree();

   /**
    * This method checks the position of objects added to the tree and
    * updates their positions in the tree sectors accordingly.
    */
   void update();

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
    * You won't be able to query this element until 
    * the tree is updated though.
    *
    * @param elem    element we want to add
    */
   void insert(Elem& elem);

   /**
    * The method allows to remove an element from a tree.
    * Unlike the addition, this time element is immediately removed
    * from the tree, however - it's extremally slow, so try avoiding it.
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

   void putElemInTree(unsigned int elemIdx, TreeElem& treeElem);

   void clearTree();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\DynamicOctree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _DYNAMIC_OCTREE_H
