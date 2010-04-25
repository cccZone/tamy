#ifndef _REGULAR_OCTREE_H
#define _REGULAR_OCTREE_H

/// @file   core\RegularOctree.h
/// @brief  an octree spatial container

#include "core\Octree.h"
#include "core\ConstSizeArray.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * An octree representation.
 */
template<typename Elem>
class RegularOctree : public Octree<Elem>
{
private:
   typedef ConstSizeArray<Elem*> ElementsArray;
   ElementsArray m_elements;

   unsigned int m_maxElemsPerSector;
   unsigned int m_maxTreeDepth;

public:
   /**
    * Constructor.
    *
    * @param treeBB              bounding box of the entire tree
    * @param maxElemsPerSector   maximum elements per sector. If there are more 
    *                            elements in a given sector than this value,
    *                            the sector gets subdivided, unless the sector
    *                            is located at the depth == maxTreeDepth.
    * @param maxTreeDepth        maximum depth of the tree
    * @param initDepth           initial depth to which the tree should be 
    *                            subdivided upon creation
    */
   RegularOctree(const AABoundingBox& treeBB, 
                 unsigned int maxElemsPerSector = 64,
                 unsigned int maxTreeDepth = 5,
                 unsigned int initDepth = 0);

   ~RegularOctree();

   // -------------------------------------------------------------------------
   // Octree implementation
   // -------------------------------------------------------------------------
   bool isAdded(const Elem& elem) const;

   void insert(Elem& elem);

   void remove(Elem& elem);

   void clear();

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
