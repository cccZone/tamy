/// @file   Tamy/DebugGeometryBuilder.h
/// @brief  debug geometrical shapes builder
#pragma once

#include "core/Math.h"


///////////////////////////////////////////////////////////////////////////////

class TriangleMesh;
struct LitVertex;
struct Face;

///////////////////////////////////////////////////////////////////////////////

class DebugGeometryBuilder
{
public:
   /**
    * Creates a line-shaped mesh with an arrow-shaped tip.
    *
    * @param size    arrow line 'width'
    * @param start   arrow start point
    * @param end     arrow tip end point
    * @return        new triangle mesh instance
    */
   static TriangleMesh* createArrow( float size, const Vector& start, const Vector& end );

   /**
    * Creates a line-shaped mesh with a box-shaped tip.
    *
    * @param size    arrow line 'width'
    * @param start   arrow start point
    * @param end     arrow tip end point
    * @return        new triangle mesh instance
    */
   static TriangleMesh* createBoxHeadedArrow( float size, const Vector& start, const Vector& end );


   /**
    * Creates a torus.
    * A torus is a ring composed of connected SEGMENTS ( number of which is controlled by the 'segmentsCount' param ).
    * Each segment has a number of vertices, the number of which you can specify using the 'segmentVerticesCount' parameter
    *
    * @param innerRadius
    * @param outerRadius
    * @param transform
    * @param segmentsCount
    * @param segmentVerticesCount
    * @return        new triangle mesh instance
    */
   static TriangleMesh* createTorus( float innerRadius, float outerRadius, const Matrix& transform, int segmentsCount, int segmentVerticesCount );

private:
   static void addCone( float baseSize, const Vector& start, const Vector& end, std::vector< LitVertex >& outVertices, std::vector< Face >& outFaces );
   static void addCuboid( float size, const Vector& start, const Vector& end, std::vector< LitVertex >& outVertices, std::vector< Face >& outFaces );
};

///////////////////////////////////////////////////////////////////////////////
