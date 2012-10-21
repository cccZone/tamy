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
   
   // <fastfloat.todo> vec optimization using select
   AABoundingBox bb;
   bb = m_bb;
   bb.max[0] = midPoint[0];
   bb.min[1] = midPoint[1];
   bb.min[2] = midPoint[2];
   m_children[CS_FRONT_LEFT_UPPER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.min[0] = midPoint[0];
   bb.min[1] = midPoint[1];
   bb.min[2] = midPoint[2];
   m_children[CS_FRONT_RIGHT_UPPER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.min[0] = midPoint[0];
   bb.max[1] = midPoint[1];
   bb.min[2] = midPoint[2];
   m_children[CS_FRONT_RIGHT_LOWER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.max[0] = midPoint[0];
   bb.max[1] = midPoint[1];
   bb.min[2] = midPoint[2];
   m_children[CS_FRONT_LEFT_LOWER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.max[0] = midPoint[0];
   bb.min[1] = midPoint[1];
   bb.max[2] = midPoint[2];
   m_children[CS_BACK_LEFT_UPPER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.min[0] = midPoint[0];
   bb.min[1] = midPoint[1];
   bb.max[2] = midPoint[2];
   m_children[CS_BACK_RIGHT_UPPER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.min[0] = midPoint[0];
   bb.max[1] = midPoint[1];
   bb.max[2] = midPoint[2];
   m_children[CS_BACK_RIGHT_LOWER] = new OctreeNode(bb, newDepth);

   bb = m_bb;
   bb.max[0] = midPoint[0];
   bb.max[1] = midPoint[1];
   bb.max[2] = midPoint[2];
   m_children[CS_BACK_LEFT_LOWER] = new OctreeNode(bb, newDepth);
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

#endif // _OCTREE_NODE_H
