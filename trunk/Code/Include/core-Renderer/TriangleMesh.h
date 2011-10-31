#ifndef _TRIANGLE_MESH_H
#define _TRIANGLE_MESH_H

/// @file   core-Renderer\TriangleMesh.h
/// @brief  geometry of a mesh composed of triangles

#include <vector>
#include "core\Resource.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\VertexArray.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\RenderResource.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is an abstract representation of a mesh geometry composed of
 * triangles.
 * Because such a mesh will preferably be stored in the graphics card's
 * memory for speed reasons, and this can only be achieved by using 
 * some low level 3D lib (DirectX, OpenGL), thus we'll put
 * this responsibility on the TriangleMeshImpl.
 */
class TriangleMesh : public GeometryResource, public RenderResource
{
   DECLARE_RESOURCE( TriangleMesh )

private:
   std::vector< LitVertex >      m_vertices;
   std::vector< Face >           m_faces;

   D3DXMATRIX                    m_identityMtx;
   AABoundingBox                 m_boundingVol;

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

   ~TriangleMesh();

   /**
    * Returns an array of vertices of the mesh (in their generic form.)
    *
    * @return  model vertices.
    */
   VertexArray* getGenericVertexArray() const;

   /**
    * Returns the faces of the model.
    *
    * @return  model faces
    */
   inline const std::vector<Face>& getFaces() const;

   // -------------------------------------------------------------------------
   // Geometry implementation
   // -------------------------------------------------------------------------
   inline  const BoundingVolume& getBoundingVolume() const { return m_boundingVol; }
   void render( Renderer& renderer );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded(ResourcesManager& mgr);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Triangle mesh rendering command.
 */
class RCRenderTriangleMesh : public RenderCommand
{
private:
   TriangleMesh&        m_mesh;

public:
   RCRenderTriangleMesh( TriangleMesh& mesh ) : m_mesh( mesh ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer\TriangleMesh.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _TRIANGLE_MESH_H
