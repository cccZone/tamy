#ifndef _REGULAR_OCTREE_H
#define _REGULAR_OCTREE_H

/// @file   core\RegularOctree.h
/// @brief  an octree spatial container

#include "core\Octree.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An octree representation.
 */
template<typename Elem>
class RegularOctree : public Octree<Elem>
{
private:
   Array<Elem*> m_elements;
   unsigned int m_maxElemsPerSector;
   unsigned int m_maxTreeDepth;

   Stack<unsigned int> m_freePos;

public:
   /**
    * Contructor.
    *
    * @param treeBB              bounding box of the entire tree
    * @param maxElemsPerSector   maximum elements per sector. If there are more 
    *                            elements in a given sector than this value,
    *                            the sector gets subdivided, unless the sector
    *                            is located at the depth == maxTreeDepth.
    * @param maxTreeDepth        maximum depth of the tree
    */
   RegularOctree(const AABoundingBox& treeBB, 
                 unsigned int maxElemsPerSector = 64,
                 unsigned int maxTreeDepth = 5);

   ~RegularOctree();

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

protected:
   unsigned int getElementsCount() const;

   Elem& getElement(unsigned int idx) const;

private:
   void spreadChildren(Sector& sector);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\RegularOctree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _REGULAR_OCTREE_H
