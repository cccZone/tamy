#ifndef _OCTREE_NODE_H
#error "This file can only be included from OctreeNode.h"
#else


///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
OctreeNode<Elem>::OctreeNode(const AABoundingBox& bb, unsigned int depth)
   : m_children(NULL)
   , m_bb(bb)
   , m_depth(depth)
{
   Vector bbMidPoint;
   m_bb.getCenter( bbMidPoint );
   m_splitPlanes[0].setFromPointNormal( bbMidPoint, Quad_0010 );
   m_splitPlanes[1].setFromPointNormal( bbMidPoint, Quad_1000 );
   m_splitPlanes[2].setFromPointNormal( bbMidPoint, Quad_0100 );
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
OctreeNode<Elem>::~OctreeNode()
{
   if (m_children != NULL)
   {
      for (int i = 0; i < 8; ++i)
      {
         delete m_children[i];
      }
   }
   delete [] m_children;
   m_children = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
bool OctreeNode<Elem>::doesIntersect(const BoundingVolume& vol) const
{
   return m_bb.testCollision(vol);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
void OctreeNode<Elem>::subdivide()
{
   if (m_children != NULL)
   {
      return;
   }

   // create child sectors
   typedef OctreeNode* OctreeNodeP;
   m_children = new OctreeNodeP[8];

   Vector midPoint;
   m_bb.getCenter( midPoint );
   
   unsigned int newDepth = m_depth + 1;
   
   AABoundingBox bb;
   bb.max.setSelect< VectorComparison::MASK_X >( midPoint, m_bb.max );
   bb.min.setSelect< VectorComparison::MASK_YZ >( midPoint, m_bb.min );
   m_children[CS_FRONT_LEFT_UPPER] = new OctreeNode( bb, newDepth );

   bb.max = m_bb.max;
   bb.min = midPoint;
   m_children[CS_FRONT_RIGHT_UPPER] = new OctreeNode( bb, newDepth );

   bb.max.setSelect< VectorComparison::MASK_Y >( midPoint, m_bb.max );
   bb.min.setSelect< VectorComparison::MASK_XZ >( midPoint, m_bb.min );
   m_children[CS_FRONT_RIGHT_LOWER] = new OctreeNode( bb, newDepth );

   bb.max.setSelect< VectorComparison::MASK_XY >( midPoint, m_bb.max );
   bb.min.setSelect< VectorComparison::MASK_Z >( midPoint, m_bb.min );
   m_children[CS_FRONT_LEFT_LOWER] = new OctreeNode( bb, newDepth );

   bb.max.setSelect< VectorComparison::MASK_XZ >( midPoint, m_bb.max );
   bb.min.setSelect< VectorComparison::MASK_Y >( midPoint, m_bb.min );
   m_children[CS_BACK_LEFT_UPPER] = new OctreeNode( bb, newDepth );

   bb.max.setSelect< VectorComparison::MASK_Z >( midPoint, m_bb.max );
   bb.min.setSelect< VectorComparison::MASK_XY >( midPoint, m_bb.min );
   m_children[CS_BACK_RIGHT_UPPER] = new OctreeNode( bb, newDepth );

   bb.max.setSelect< VectorComparison::MASK_YZ >( midPoint, m_bb.max );
   bb.min.setSelect< VectorComparison::MASK_X >( midPoint, m_bb.min );
   m_children[CS_BACK_RIGHT_LOWER] = new OctreeNode( bb, newDepth );

   bb.max = midPoint;
   bb.min = m_bb.min;
   m_children[CS_BACK_LEFT_LOWER] = new OctreeNode( bb, newDepth );
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int OctreeNode<Elem>::getChildrenCount() const
{
   if (m_children == NULL)
   {
      return 0;
   }
   else
   {
      return 8;
   }
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
OctreeNode<Elem>& OctreeNode<Elem>::getChild(unsigned int idx)
{
   ASSERT_MSG (m_children != NULL, "This is a leaf node - it doesn't have any children");
   ASSERT_MSG (idx < 8, "Child node index out of range");

   return *(m_children[idx]);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
const OctreeNode<Elem>& OctreeNode<Elem>::getChild(unsigned int idx) const
{
   ASSERT_MSG (m_children != NULL, "This is a leaf node - it doesn't have any children");
   ASSERT_MSG (idx < 8, "Child node index out of range");

   return *(m_children[idx]);
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
const Plane& OctreeNode<Elem>::getSplitPlane(unsigned int idx) const
{
   ASSERT_MSG (idx < 3, "Split plane index out of range");
   return m_splitPlanes[idx];
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
unsigned int OctreeNode<Elem>::getDepth() const
{
   return m_depth;
}

///////////////////////////////////////////////////////////////////////////////

template<typename Elem>
const AABoundingBox& OctreeNode<Elem>::getBoundingBox() const
{
   return m_bb;
}

///////////////////////////////////////////////////////////////////////////////

template< typename Elem > template< typename OctreeElem >
void OctreeNode< Elem >::recalculateGlobalBounds( const Octree< OctreeElem >& octree )
{
   // first step down into the hierarchy and recalculate its bounds
   int count = getChildrenCount();
   for ( int i = 0; i < count; ++i )
   {
      OctreeNode< Elem >& child = getChild(i);
      child.recalculateGlobalBounds( octree );
   }

   // next, recalculate the local bounds
   recalculateLocalBounds( octree );

   // and finally - combine all bounds into the node's global bounds
   m_globalElementsBounds = m_localElementsBounds;
   for ( int i = 0; i < count; ++i )
   {
      OctreeNode< Elem >& child = getChild(i);
      m_globalElementsBounds.add( child.m_globalElementsBounds, m_globalElementsBounds );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename Elem > template< typename OctreeElem >
void OctreeNode<Elem>::recalculateLocalBounds( const Octree< OctreeElem >& octree )
{
   AABoundingBox elemBB;

   // calculate the bounding box around this node's elements
   m_localElementsBounds.reset();
   uint count = m_elems.size();
   for ( uint i = 0; i < count; ++i )
   {
      uint elemIdx = m_elems[i];
      const BoundingVolume& vol = octree.getElement(elemIdx).getBoundingVolume();
      vol.calculateBoundingBox( elemBB );

      m_localElementsBounds.add( elemBB, m_localElementsBounds );
   }

}

///////////////////////////////////////////////////////////////////////////////

#endif // _OCTREE_NODE_H
