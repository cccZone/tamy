#ifndef _TRIANGLE_SPLITTER_H
#define _TRIANGLE_SPLITTER_H

/// @file   core\TriangleSplitter.h
/// @brief  utility for splitting triangles with a plane

#include "core\Array.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a helper class to the TriangleSplitter.
 * It specifies an interface the splitter class will require
 * from a triangle in order to split it.
 */
template<typename VERTEX>
class SplittableTriangle
{
public:
   /**
    * This method returns a vertex with the specified index.
    *
    * @param vtxIdx  index of the vertex <0, 2>
    * @return        the vertex itself
    */
   const VERTEX& vertex(unsigned int vtxIdx) const;

   /**
    * This method returns position of the specified vertex.
    *
    * @param vtxIdx  index of the vertex <0, 2>
    * @return        position of the vertex
    */
   const D3DXVECTOR3& vertexPos(unsigned int vtxIdx) const;

   /**
    * The method creates a new vertex somewhere on the specified edge.
    *
    * @param percentage    percentage of distance from start vertex
    *                      to end vertex where the new one will be created.
    * @param startVtxIdx   the beginning of the edge
    * @param endVtxIdx     the end of the edge
    */
   VERTEX splitEdge(float percentage, 
                    unsigned int startVtxIdx, 
                    unsigned int endVtxIdx) const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This algorithm can be used to split triangles
 * in half using an arbitrary plane.
 *
 * One can split various types of triangles that can be composed of
 * various types of vertices. Simply specify what kind of vertex does
 * your triangle use and the type of the triangle you wish to obtain.
 * Make sure that the triangle type you specified has a constructor
 * accepting 3 vertices:
 *    TRIANGLE(const VERTEX& v1, const VERTEX& v2, const VERTEX& v3);
 */
template <typename VERTEX, typename TRIANGLE>
class TriangleSplitter
{
public:
   /**
    * The method will split a triangle using the specified plane.
    * A single split can result in creation of up to three triangles.
    *
    * If the plane doesn't intersect the triangle, only one triangle
    * will be created and it will be assigned to a variable classifying
    * whether the new triangle was in front or behind the plane.
    *
    * @param triangle   triangle we wish to split. Only instances of the helper
    *                   class 'SplittableTriangle' are accepted, so that 
    *                   the method is guaranteed to get all the functionality
    *                   from the triangle it requires.
    * @param plane      plane which we want to use for splitting
    * @param front      this is an output parameter and it is where the method
    *                   will store new triangles that are in front of the split
    *                   plane
    * @param back       this is an output parameter and it is where the method
    *                   will store new triangles that are behind the split
    *                   plane
    */
   void split(const SplittableTriangle<VERTEX>& triangle,
              const D3DXPLANE& plane,
              Array<TRIANGLE*>& front,
              Array<TRIANGLE*>& back) const;

private: 
   VERTEX splitEdge(float percentage,
                    const VERTEX& v1, 
                    const VERTEX& v2) const;
   void triangulatePoly(const Array<VERTEX>& poly, 
                        typename Array<TRIANGLE*>& output) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core\TriangleSplitter.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _TRIANGLE_SPLITTER_H
