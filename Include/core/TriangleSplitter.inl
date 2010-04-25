#ifndef _TRIANGLE_SPLITTER_H
#error "This file can only be included from TriangleSplitter.h"
#else

///////////////////////////////////////////////////////////////////////////////

template <typename VERTEX, typename TRIANGLE>
void TriangleSplitter<VERTEX, TRIANGLE>::split(const SplittableTriangle<VERTEX>& triangle,
                                               const D3DXPLANE& plane, 
                                               Array<TRIANGLE*>& front,
                                               Array<TRIANGLE*>& back) const
{
   static float distanceEps = 0;

   Array<VERTEX> frontPoly(4);
   Array<VERTEX> backPoly(4);

   // split the triangle into two polygons
   D3DXVECTOR3 planeNormal(plane.a, plane.b, plane.c);
   for (unsigned int i = 0; i < 3; ++i)
   {
      const D3DXVECTOR3& analyzedVtxPos = triangle.vertexPos(i);
      const VERTEX& analyzedVtx = triangle.vertex(i);
      float prevDist = D3DXVec3Dot(&analyzedVtxPos, &planeNormal) + plane.d;

      if (prevDist > distanceEps)
      {
         frontPoly.push_back(analyzedVtx);
      }
      else if (prevDist < -distanceEps)
      {
         backPoly.push_back(analyzedVtx);
      }
      else
      {
         frontPoly.push_back(analyzedVtx);
         backPoly.push_back(analyzedVtx);
      }

      unsigned int nextVtxIdx = (i + 1) % 3;
      float nextDist = D3DXVec3Dot(&triangle.vertexPos(nextVtxIdx), &planeNormal) + plane.d;
      if ((nextDist * prevDist) < -distanceEps)
      {
         float distToV1 = fabs(prevDist);
         float distToV2 = fabs(nextDist);
         float percentage = distToV1 / (distToV1 + distToV2);
         VERTEX midVtx = triangle.splitEdge(percentage, i, nextVtxIdx);
         frontPoly.push_back(midVtx);
         backPoly.push_back(midVtx);
      }
   }

   // triangulate both
   triangulatePoly(frontPoly, front);
   triangulatePoly(backPoly, back);
}

///////////////////////////////////////////////////////////////////////////////

template <typename VERTEX, typename TRIANGLE>
void TriangleSplitter<VERTEX, TRIANGLE>::triangulatePoly(const Array<VERTEX>& poly, 
                                               typename Array<TRIANGLE*>& output) const
{
   unsigned int verticesCount = poly.size();
   switch(verticesCount)
   {
   case 3:
      {
         output.push_back(new TRIANGLE(poly[0], poly[1], poly[2]));
         break;
      }

   case 4:
      {
         output.push_back(new TRIANGLE(poly[0], poly[1], poly[2]));
         output.push_back(new TRIANGLE(poly[0], poly[2], poly[3]));
         break;
      }
   default:
      break;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TRIANGLE_SPLITTER_H
