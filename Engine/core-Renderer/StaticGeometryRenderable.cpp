#include "core-Renderer\StaticGeometryRenderable.h"
#include "core\Triangle.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

StaticGeometryRenderable::StaticGeometryRenderable(Material& material,
                                                   const std::vector<LitVertex>& vertices,
                                                   const std::vector<Face>& faces)
: m_material(material)
, m_vertices(vertices)
, m_faces(faces)
{
   D3DXMATRIX globalMtx;
   D3DXMatrixIdentity(&globalMtx);
   m_renderingMtcs.push_back(globalMtx);

   // calculate the bounding volume
   D3DXVECTOR3 minCoord(FLT_MAX, FLT_MAX, FLT_MAX);
   D3DXVECTOR3 maxCoord(-FLT_MAX, -FLT_MAX, -FLT_MAX);
   unsigned int verticesCount = vertices.size();
   for (unsigned int i = 0; i < verticesCount; ++i)
   {
      if      (vertices[i].m_coords.x < minCoord.x) 
      {minCoord.x = vertices[i].m_coords.x;}
      else if (vertices[i].m_coords.x > maxCoord.x) 
      {maxCoord.x = vertices[i].m_coords.x;}

      if      (vertices[i].m_coords.y < minCoord.y) 
      {minCoord.y = vertices[i].m_coords.y;}
      else if (vertices[i].m_coords.y > maxCoord.y) 
      {maxCoord.y = vertices[i].m_coords.y;}

      if      (vertices[i].m_coords.z < minCoord.z) 
      {minCoord.z = vertices[i].m_coords.z;}
      else if (vertices[i].m_coords.z > maxCoord.z) 
      {maxCoord.z = vertices[i].m_coords.z;}
   }
   m_boundingVol.min = minCoord;
   m_boundingVol.max = maxCoord;

   // find the situation
   D3DXVECTOR3 situationPos = (m_boundingVol.min + m_boundingVol.max) / 2.f;
   D3DXMatrixTranslation(&m_situation, situationPos.x, situationPos.y, situationPos.z);
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryRenderable::~StaticGeometryRenderable()
{
}

///////////////////////////////////////////////////////////////////////////////

const std::vector<LitVertex>& StaticGeometryRenderable::getVertices() const
{
   return m_vertices;
}

///////////////////////////////////////////////////////////////////////////////

const std::vector<Face >& StaticGeometryRenderable::getFaces() const
{
   return m_faces;
}

///////////////////////////////////////////////////////////////////////////////

const Array<D3DXMATRIX>& StaticGeometryRenderable::getRenderingMatrices() 
{
   return m_renderingMtcs;
}

///////////////////////////////////////////////////////////////////////////////

const Material& StaticGeometryRenderable::getMaterial() const 
{
   return m_material;
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& StaticGeometryRenderable::getVolume()
{
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& StaticGeometryRenderable::getGlobalTransform() 
{
   return m_situation;
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryRenderable::split(const D3DXPLANE& splitPlane, 
                                     StaticGeometryRenderable** frontSplit,
                                     StaticGeometryRenderable** backSplit)
{
   unsigned int trianglesCount = m_faces.size();

   Array<MeshTriangle*> backTriangles(trianglesCount);
   Array<MeshTriangle*> frontTriangles(trianglesCount);
   for (unsigned int i = 0; i < trianglesCount; ++i)
   {
      MeshTriangle triangle(m_vertices[m_faces[i].idx[0]],
                                  m_vertices[m_faces[i].idx[1]],
                                  m_vertices[m_faces[i].idx[2]]);
      m_splitter.split(triangle, splitPlane, frontTriangles, backTriangles);
   }

   std::vector<LitVertex> vertices;
   std::vector<Face > faces;

   if (frontTriangles.size() > 0) 
   {
      createNewObject(frontTriangles, vertices, faces);
      *frontSplit = createSelf(m_material, vertices, faces);
   }
   else
   {
      *frontSplit = NULL;
   }

   if (backTriangles.size() > 0) 
   {
      vertices.clear(); faces.clear();
      createNewObject(backTriangles, vertices, faces);
      *backSplit = createSelf(m_material, vertices, faces);
   }
   else
   {
      *backSplit = NULL;
   }

   // cleanup
   unsigned int tmpTrianglesCount = backTriangles.size();
   for (unsigned int i = 0; i < tmpTrianglesCount; ++i) {delete backTriangles[i];}
   tmpTrianglesCount = frontTriangles.size();
   for (unsigned int i = 0; i < tmpTrianglesCount; ++i) {delete frontTriangles[i];}
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& StaticGeometryRenderable::getBoundingVolume() 
{
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryRenderable::createNewObject(const Array<MeshTriangle*>& triangles,
                                               std::vector<LitVertex>& outVertices,
                                               std::vector<Face >& outFaces)
{
   unsigned int trianglesCount = triangles.size();
   MeshTriangle* triangle;
   unsigned int idx;
   for (unsigned int i = 0; i < trianglesCount; ++i)
   {
      triangle = triangles[i];
      outVertices.push_back(triangle->vtx[0]);
      outVertices.push_back(triangle->vtx[1]);
      outVertices.push_back(triangle->vtx[2]);

      idx = i * 3;
      outFaces.push_back(Face(idx, idx + 1, idx + 2, 0));
   }
}

///////////////////////////////////////////////////////////////////////////////
