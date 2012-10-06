/// @file   core\BSPNodeTree.h
/// @brief  a BSP tree that stores polygons in its nodes
#ifndef _BSP_NODE_TREE_H
#define _BSP_NODE_TREE_H

#include "core\MemoryUtils.h"
#include "core\Array.h"
#include "core\Plane.h"


///////////////////////////////////////////////////////////////////////////////

struct Vector;

///////////////////////////////////////////////////////////////////////////////

/**
 * This BSP tree stores split polygons in its nodes, and thus is perfect
 * for implementing renderables that require sorted geometry (like transparent
 * geometry for example).
 */
template <class Polygon>
class BSPNodeTree
{
   ALIGNED_CLASS();

private:
   struct Node
   {
      ALIGNED_STRUCT();

      Node* back;
      Node* front;
      Plane splitPlane;
      Array<Polygon*> geometry;

      Node();
      ~Node();
   };

private:
   Node* m_root;

public:
   /**
    * Constructor.
    *
    * @param geometry   geometry we want to build the tree from.
    */
   BSPNodeTree(const Array<Polygon*>& geometry);
   ~BSPNodeTree();

   /**
    * Queries the tree for polygons it contains. Those polygons will
    * be returned sorted with respect to the query position.
    *
    * @param pos           position with respect to which the polygons
    *                      should be sorted
    * @param results       placeholder for results
    * @param checkedNode   internal parameter specifying from which node
    *                      should the query start. Better left unspecified
    *                      if you want to get polygons from the whole tree.
    */
   void query( const Vector& pos, Array<Polygon*>& results, Node* checkedNode = NULL );

private:
   void createSubtree(Node* currNode, const Array<Polygon*>& geometry);

   void classifyPolygonAgainstPlane( const Plane& splittingPlane, Polygon* classifiedPoly, Node* currNode, Array<Polygon*>& outBackPolys, Array<Polygon*>& outFrontPolys );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\BSPNodeTree.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _BSP_NODE_TREE_H
