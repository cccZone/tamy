#ifndef _BSP_NODE_TREE_H
#error "This file can only be included from BSPNodeTree.h"
#else

#include "core\PointVolume.h"


///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
BSPNodeTree<Polygon>::BSPNodeTree( const Array<Polygon*>& geometry )
{
   if ( geometry.size() == 0 ) 
   {
      return;
   }

   m_root = new Node();
   createSubtree(m_root, geometry);
}

///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
BSPNodeTree<Polygon>::~BSPNodeTree()
{
   delete m_root; m_root = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
void BSPNodeTree<Polygon>::query(const Vector& pos, Array<Polygon*>& results, Node* checkedNode )
{
   PointVolume queriedPos( pos );
   if ( checkedNode == NULL ) 
   {
      checkedNode = m_root;
   }

   const FastFloat planeDist = queriedPos.distanceToPlane( checkedNode->splitPlane );

   // determine the order in which we're gonna traverse the subtree
   Node* nodeToCheckFirst;
   Node* nodeToCheckSecond;
   if ( planeDist < Float_0 ) // behind the plane
   {
      nodeToCheckFirst = checkedNode->front;
      nodeToCheckSecond = checkedNode->back;
   }
   else // in front of or on the plane
   {
      nodeToCheckFirst = checkedNode->back;
      nodeToCheckSecond = checkedNode->front;
   }

   // query the subtree
   if ( nodeToCheckFirst != NULL )
   {
      query( pos, results, nodeToCheckFirst );
   }

   results.copyFrom( checkedNode->geometry );

   if ( nodeToCheckSecond != NULL )
   {
      query( pos, results, nodeToCheckSecond );
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
void BSPNodeTree<Polygon>::createSubtree( Node* currNode, const Array<Polygon*>& geometry )
{
   unsigned int selectedSplitPolyIdx = 0;
   Polygon* splittingPolygon = geometry[selectedSplitPolyIdx];

   splittingPolygon->getPlane( currNode->splitPlane );

   currNode->geometry.push_back(splittingPolygon);

   Array<Polygon*> backPolys;
   Array<Polygon*> frontPolys;
   unsigned int polysCount = geometry.size();
   for (unsigned int i = 0; i < polysCount; ++i)
   {
      if (selectedSplitPolyIdx == i) 
      {
         continue;
      }
      classifyPolygonAgainstPlane( currNode->splitPlane, geometry[i], currNode, backPolys, frontPolys );
   }

   if (backPolys.size() > 0)
   {
      currNode->back = new Node();
      createSubtree(currNode->back, backPolys);
   }

   if (frontPolys.size() > 0)
   {
      currNode->front = new Node();
      createSubtree(currNode->front, frontPolys);
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
void BSPNodeTree<Polygon>::classifyPolygonAgainstPlane(const Plane& splittingPlane, Polygon* classifiedPoly, Node* currNode, Array<Polygon*>& outBackPolys, Array<Polygon*>& outFrontPolys)
{
   PlaneClassification classification = classifiedPoly->classifyAgainst(splittingPlane);
   switch (classification)
   {
   case PPC_BACK:
      {
         outBackPolys.push_back(classifiedPoly);
         break;
      }

   case PPC_FRONT:
      {
         outFrontPolys.push_back(classifiedPoly);
         break;
      }

   case PPC_COPLANAR:
      {
         currNode->geometry.push_back(classifiedPoly);
         break;
      }

   case PPC_SPANNING:
      {
         Array<Polygon*> frontSplit;
         Array<Polygon*> backSplit;
         classifiedPoly->split(splittingPlane, frontSplit, backSplit);
         delete classifiedPoly;

         if (frontSplit.size() > 0) {outFrontPolys.copyFrom(frontSplit);}
         if (backSplit.size() > 0) {outBackPolys.copyFrom(backSplit);}
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
BSPNodeTree<Polygon>::Node::Node() 
   : back(NULL)
   , front(NULL) 
{}

///////////////////////////////////////////////////////////////////////////////

template <class Polygon>
BSPNodeTree<Polygon>::Node::~Node() 
{
   delete back; back = NULL;
   delete front; front = NULL;

   unsigned int count = geometry.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete geometry[i];
   }
   geometry.clear();                       
}

///////////////////////////////////////////////////////////////////////////////

#endif // _BSP_NODE_TREE_H
