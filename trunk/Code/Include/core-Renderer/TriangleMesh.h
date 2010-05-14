#ifndef _TRIANGLE_MESH_H
#define _TRIANGLE_MESH_H

/// @file   core-Renderer\TriangleMesh.h
/// @brief  geometry of a mesh composed of triangles

#include <vector>
#include "core\Resource.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\VertexArray.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"
#include "core\AABoundingBox.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"

///////////////////////////////////////////////////////////////////////////////

/**
 * Graphics library dependent implementation of the triangle mesh.
 */
class TriangleMeshImpl : public RendererObjectImpl
{
public:
   virtual ~TriangleMeshImpl() {}

   /**
    * Renders the mesh using the library calls.
    */
   virtual void render() {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This is an abstract representation of a mesh geometry composed of
 * triangles.
 * Because such a mesh will preferably be stored in the graphics card's
 * memory for speed reasons, and this can only be achieved by using 
 * some low level 3D lib (DirectX, OpenGL), thus we'll put
 * this responsibility on the TriangleMeshImpl.
 */
class TriangleMesh : public GeometryResource, 
                     public TRendererObject<TriangleMeshImpl>
{
   DECLARE_RESOURCE( TriangleMesh )

private:
   std::vector<LitVertex> m_vertices;
   std::vector<Face> m_faces;

   D3DXMATRIX m_identityMtx;
   AABoundingBox m_boundingVol;

public:
   /**
    * Default constructor.
    */
   TriangleMesh();

   /**
    * Constructor.
    *
    * @param name       resource name
    * @param vertices   mesh vertices
    * @param faces      mesh faces
    */
   TriangleMesh( const std::string& name,
                 const std::vector<LitVertex>& vertices,
                 const std::vector<Face>& faces );

   /**
    * Returns an array of vertices of the mesh (in their generic form.)
    *
    * @return  model vertices.
    */
   VertexArray* getGenericVertexArray();

   /**
    * Returns the faces of the model.
    *
    * @return  model faces
    */
   inline const std::vector<Face>& getFaces() const;

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   BoundingVolume* calculateBoundingVolume() const;

   void render();

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded(ResourcesManager& mgr);
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\TriangleMesh.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _TRIANGLE_MESH_H
