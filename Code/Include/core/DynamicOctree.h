#ifndef _DYNAMIC_OCTREE_H
#define _DYNAMIC_OCTREE_H

/// @file   core\DynamicOctree.h
/// @brief  an octree capable of storing dynamic (moving) objects

#include "core\Octree.h"
#include "core\ConstSizeArray.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An octree that can store dynamic objects and regenerates itself
 * whenever the objects change their location.
 */
template<typename Elem>
class DynamicOctree : public Octree<Elem>
{
private:
   /**
    * A helper structure for storing elements added to the tree.
    */
   struct TreeElem
   {
      Elem* elem;
      Array<Sector*> hostSectors;

      TreeElem(Elem* _elem);
      ~TreeElem();
   };

private:
   typedef ConstSizeArray<TreeElem*> ElementsArray;
   ElementsArray m_elements;

public:
   /**
    * Constructor. 
    * The tree is pre-created here - no further subdivision
    * will happen after the tree's been created.
    *
    * @param treeBB     bounding box for the tree
    * @param treeDepth  depth at which we want the tree to have its leaves.
    *                   Depth == 0 is allowed and means that the tree has
    *                   only one node where everything is stored - thus it
    *                   becomes nothing more than a mere linear storage.
    */
   DynamicOctree(const AABoundingBox& treeBB, int treeDepth);

   ~DynamicOctree();

   /**
    * This method checks the position of an object.
    *
    * @param elem    object in the tree
    */
   void update(Elem& elem);

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

protected:
   unsigned int getElementsCount() const;

   Elem& getElement(unsigned int idx) const;

private:
   void putElemInTree(unsigned int elemIdx, TreeElem& treeElem);

   void clearTree();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\DynamicOctree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _DYNAMIC_OCTREE_H
