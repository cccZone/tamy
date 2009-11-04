#ifndef _STATIC_GEOMETRY_OCTREE_H
#define _STATIC_GEOMETRY_OCTREE_H

/// @file   core\StaticGeometryOctree.h
/// @brief  an octree for storing static geometry

#include "core\Octree.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This octree is specialized for storing static geometry.
 * It requires from elements that are added to it an ability
 * to be subdivided if they span across sector boundaries,
 * and for this reason only geometry related elements fall into
 * the domain of this tree elements.
 *
 * All elements added to this storage, apart from the methods
 * described in the Octree class description, require an additional
 * method:
 *   void split(const D3DXPLANE& splitPlane, 
 *              Elem** outFrontSplit, 
 *              Elem** outBackSplit);
 *
 * This method should split the entity using the specified splitPlane,
 * and return the results using two params: 'outFrontSplit', 'outBackSplit',
 * passing NULL in case a new element wasn't created in the corresponding 
 * subspace.
 */
template<typename Elem, typename ResultingGeometryStorage>
class StaticGeometryOctree : public Octree<Elem>
{
private:
   Array<Elem*> m_elements;
   Stack<unsigned int> m_freeSpaces;

   unsigned int m_maxElemsPerSector;
   unsigned int m_maxDepth;

   ResultingGeometryStorage& m_outputGeometry;

public:
   /**
    * Constructor. 
    *
    * @param treeBB           bounding box for the tree
    * @param maxElements      when a new element is added to a sector,
    *                         the sector will check whether it doesn't exceed
    *                         this value in terms of how many elements it stores.
    *                         If it does, it will be further subdivided
    * @param maxDepth         maximum depth of the tree. Let's assume we're
    *                         adding 4 objects that have the same bounding volume
    *                         and are located on the same position, while maxElems
    *                         is set to 1. In this case, the tree would try to infinitely
    *                         subdivide itself and thus fall into an endless loop.
    *                         To prevent this from happening, this param comes
    *                         into play. It also helps stabilizing query times, by
    *                         preventing the tree to be very tall in one place, and
    *                         ultra shallow in another.
    * @param outputGeometry   The tree's gonna perform geometry splitting. This is where
    *                         the resulting geometry be held.
    * @param initDepth        for convenience tree can be initially subdivided to
    *                         a certain depth. This depth is limited by the value
    *                         specified in 'maxDepth' param
    */
   StaticGeometryOctree(const AABoundingBox& treeBB, 
                        int maxElements, 
                        int maxDepth,
                        ResultingGeometryStorage& outputGeometry,
                        int initDepth = 0);

   ~StaticGeometryOctree();

   /**
    * The method checks if an element is present in the tree.
    *
    * @param elem    element presence of which we want to check
    * @return        'true' if element was successfully added,
    *                'false' otherwise
    */
   bool isAdded(const Elem& elem) const;

   /**
    * The method inserts a new element into the tree, splitting
    * it if the element crosses the boundaries of sectors.
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

   void addElemToTree(Elem& elem, 
                      Sector& subTreeRoot,
                      Array<Sector*>& changedSectors);

   void performSectorSubdivision(Sector& sector);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\StaticGeometryOctree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _STATIC_GEOMETRY_OCTREE_H
