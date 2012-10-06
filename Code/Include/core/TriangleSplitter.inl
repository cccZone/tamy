#ifndef _TRIANGLE_SPLITTER_H
#error "This file can only be included from TriangleSplitter.h"
#else

#include "core/FastFloat.h"


///////////////////////////////////////////////////////////////////////////////

template <typename VERTEX, typename TRIANGLE>
void TriangleSplitter<VERTEX, TRIANGLE>::split( const SplittableTriangle<VERTEX>& triangle, const Plane& plane, Array<TRIANGLE*>& front, Array<TRIANGLE*>& back ) const
{
   FastFloat posDistanceEps, negDistanceEps;
   posDistanceEps = Float_0;
   negDistanceEps.setNeg( Float_0 );

   Array<VERTEX> frontPoly(4);
   Array<VERTEX> backPoly(4);

   // split the triangle into two polygons
   for (unsigned int i = 0; i < 3; ++i)
   {
      const Vector& analyzedVtxPos = triangle.vertexPos(i);
      const VERTEX& analyzedVtx = triangle.vertex(i);
      const FastFloat prevDist = plane.dotCoord( analyzedVtxPos );

      if ( prevDist > posDistanceEps )
      {
         frontPoly.push_back( analyzedVtx );
      }
      else if ( prevDist < negDistanceEps )
      {
         backPoly.push_back( analyzedVtx );
      }
      else
      {
         frontPoly.push_back( analyzedVtx );
         backPoly.push_back( analyzedVtx );
      }

      unsigned int nextVtxIdx = ( i + 1 ) % 3;
      const FastFloat nextDist = plane.dotCoord( triangle.vertexPos( nextVtxIdx ) );
      FastFloat distDot;
      distDot.setMul( nextDist, prevDist );

      if ( distDot < negDistanceEps )
      {
         FastFloat distToV1, distToV2, percentage;
         distToV1.setAbs( prevDist );
         distToV2.setAbs( nextDist );
         percentage.setAdd( distToV1, distToV2 );
         percentage.reciprocal();
         percentage.mul( distToV1 );
         VERTEX midVtx;
         triangle.splitEdge( percentage, i, nextVtxIdx, midVtx );
         frontPoly.push_back( midVtx );
         backPoly.push_back( midVtx );
      }
   }

   // triangulate both
   triangulatePoly( frontPoly, front );
   triangulatePoly( backPoly, back );
}

///////////////////////////////////////////////////////////////////////////////

template <typename VERTEX, typename TRIANGLE>
void TriangleSplitter<VERTEX, TRIANGLE>::triangulatePoly( const Array<VERTEX>& poly, typename Array<TRIANGLE*>& output ) const
{
   unsigned int verticesCount = poly.size();
   switch(verticesCount)
   {
   case 3:
      {
         output.push_back( new TRIANGLE(poly[0], poly[1], poly[2] ) );
         break;
      }

   case 4:
      {
         output.push_back( new TRIANGLE( poly[0], poly[1], poly[2] ) );
         output.push_back( new TRIANGLE( poly[0], poly[2], poly[3] ) );
         break;
      }
   default:
      break;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TRIANGLE_SPLITTER_H
