#include "dx9-Renderer\DX9TriangleMesh.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <stdexcept>

#include "core-Renderer\LitVertex.h"

///////////////////////////////////////////////////////////////////////////////

DX9TriangleMesh::DX9TriangleMesh(TriangleMesh& mesh)
: m_mesh( mesh )
, m_d3Device( NULL )
, m_vb( NULL )
, m_ib( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9TriangleMesh::~DX9TriangleMesh()
{
   if ( m_vb != NULL)
   {
      m_vb->Release();
      m_vb = NULL;
   }

   if ( m_ib != NULL)
   {
      m_ib->Release();
      m_ib = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9TriangleMesh::render()
{
   if ( m_vb && m_ib )
   {
      m_d3Device->SetStreamSource( 0, m_vb, 0, sizeof( LitVertex ) );
      m_d3Device->SetIndices( m_ib );
      m_d3Device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_verticesCount, 0, m_facesCount );
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9TriangleMesh::initialize(Renderer& renderer)
{
   DX9Renderer* d3dRenderer = dynamic_cast<DX9Renderer*> (&renderer);
   if (d3dRenderer == NULL)
   {
      throw std::runtime_error("This class works only with DX9Renderer instance");
   }

   m_d3Device = &d3dRenderer->getD3Device();

   VertexArray* vertices = m_mesh.getGenericVertexArray();
   const std::vector<Face>& faces = m_mesh.getFaces();
   unsigned int verticesCount = vertices->size();
   unsigned int trianglesCount = faces.size();

   if ((verticesCount == 0) || (trianglesCount == 0))
   {
      delete vertices;
      return;
   }

   ID3DXMesh* dxMesh = NULL;
   DWORD FVF = vertices->getFVF();
   HRESULT res = D3DXCreateMeshFVF( trianglesCount, verticesCount, 
                                    D3DXMESH_MANAGED, FVF, 
                                    &(d3dRenderer->getD3Device()), &dxMesh );
   if ( FAILED(res) ) 
   { 
      throw std::logic_error("Can't create a mesh"); 
   }

   // fill the vertex buffer, analyze the bounding sphere radius on the way
   void* pVertex = NULL;
   res = dxMesh->LockVertexBuffer( 0, (void**)&pVertex );
   if ( FAILED( res ) ) 
   { 
      throw std::logic_error( "Can't lock the mesh's vertex buffer" );
   }
   vertices->copyTo( pVertex );
   dxMesh->UnlockVertexBuffer();

   // fill the index buffer & the attributes table
   USHORT* pIndex = NULL;
   DWORD* pAttrib = NULL;
   res = dxMesh->LockIndexBuffer( 0, (void**)&pIndex );
   if ( FAILED( res ) ) 
   { 
      throw std::logic_error("Can't lock the mesh's index buffer"); 
   }
   res = dxMesh->LockAttributeBuffer(0, &pAttrib );
   if ( FAILED( res ) ) 
   { 
      throw std::logic_error("Can't lock the mesh's attributes buffer");
   }

   for ( unsigned int i = 0; i < trianglesCount; ++i )
   {
      const Face& face = faces[i];
      *pIndex++  = face.idx[0]; 
      *pIndex++  = face.idx[1]; 
      *pIndex++  = face.idx[2];
      *pAttrib++ = 0;
   }
   dxMesh->UnlockIndexBuffer();
   dxMesh->UnlockAttributeBuffer();
   /*
   if (trianglesCount > 2)
   {
      DWORD *adjacency = new DWORD[ dxMesh->GetNumFaces() * 3 ];
      dxMesh->GenerateAdjacency( 0.001f, adjacency );
      dxMesh->OptimizeInplace( D3DXMESHOPT_VERTEXCACHE, adjacency, NULL, NULL, NULL );
      delete [] adjacency;
   }
   */
   delete vertices;

   // copy the buffers
   dxMesh->GetVertexBuffer( &m_vb );
   dxMesh->GetIndexBuffer( &m_ib );
   m_verticesCount = dxMesh->GetNumVertices();
   m_facesCount = dxMesh->GetNumFaces();
   dxMesh->Release();
}

///////////////////////////////////////////////////////////////////////////////
