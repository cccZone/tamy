#ifndef _STATIC_GEOMETRY_OCTREE_H
#define _STATIC_GEOMETRY_OCTREE_H

/// @file   core\StaticGeometryOctree.h
/// @brief  an octree for storing static geometry

#include "core\Octree.h"
#include "core\ConstSizeArray.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

typedef unsigned int SGHandle;
typedef std::vector<unsigned int> SGElementParts;

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
template<typename Elem>
class StaticGeometryOctree : public Octree<Elem>
{
private:
   struct SGElement
   {
      /* This is the handle assigned to the original element.
       * Since it can be split into many sub elements, we want to 
       * keep track which elements comprise iterator. */
      SGHandle handle;

      Elem* elem;

      SGElement(SGHandle _handle, Elem* _elem)
         : handle(_handle), elem(_elem)
      {}

      ~SGElement()
      {
         delete elem; elem = NULL;
      }
   };

private:
   typedef ConstSizeArray<SGElement*> ElementsArray;
   ElementsArray m_elements;

   typedef ConstSizeArray<SGElementParts*> HandlesArray;
   HandlesArray m_handles;
   
   unsigned int m_maxElemsPerSector;
   unsigned int m_maxDepth;

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
    * @param initDepth        for convenience tree can be initially subdivided to
    *                         a certain depth. This depth is limited by the value
    *                         specified in 'maxDepth' param
    */
   StaticGeometryOctree(const AABoundingBox& treeBB, 
                        int maxElements, 
                        int maxDepth,
                        int initDepth = 0);

   ~StaticGeometryOctree();

   /**
    * The method inserts a new element into the tree, splitting
    * it if the element crosses the boundaries of sectors.
    *
    * @param elem    element we want to add
    * @return        handle to the element we've just added
    */
   SGHandle insert(Elem* elem);

   /**
    * The method allows to remove an element from a tree. The element 
    * will be destroyed.
    *
    * @param elemHandle    handle to an element we want to remove
    */
   void remove(SGHandle elemHandle);

   /**
    * This method returns the number of elements in the tree, 
    * counting each split part as a separate element.
    *
    * @return     number of elements in the tree including splits
    */
   unsigned int getSplitElementsCount() const;

protected:
   // -------------------------------------------------------------------------
   // Octree implementation
   // -------------------------------------------------------------------------
   unsigned int getElementsCount() const;

   Elem& getElement(unsigned int idx) const;

protected:
   void addElemToTree(SGElement* element, 
                      Sector& subTreeRoot,
                      Array<Sector*>& changedSectors);

   void performSectorSubdivision(Sector& sector);

   // -------------------------------------------------------------------------
   // Elements management
   // -------------------------------------------------------------------------
   unsigned int addElement(SGElement* element);
   SGElement* releaseElement(unsigned int idx);

   // -------------------------------------------------------------------------
   // Handles management
   // -------------------------------------------------------------------------
   SGHandle addHandle();
   void releaseHandle(SGHandle idx);
};

///////////////////////////////////////////////////////////////////////////////

#include "core\StaticGeometryOctree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _STATIC_GEOMETRY_OCTREE_H
