#include "dx9-Renderer\DX9TriangleMesh.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\LitVertex.h"


///////////////////////////////////////////////////////////////////////////////

void RCRenderTriangleMesh::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );
   DX9TriangleMesh* mesh = dxRenderer.getTriangleMesh( m_mesh );
   if ( mesh )
   {
      mesh->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

DX9TriangleMesh::DX9TriangleMesh( const DX9Renderer& renderer, const TriangleMesh& mesh )
: m_mesh( mesh )
, m_renderer( renderer )
, m_d3Device( &renderer.getD3Device() )
, m_vb( NULL )
, m_ib( NULL )
{
   initialize();
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

void DX9TriangleMesh::initialize()
{
   VertexArray* vertices = m_mesh.getGenericVertexArray();
   const std::vector< Face >& faces = m_mesh.getFaces();
   unsigned int verticesCount = vertices->size();
   unsigned int trianglesCount = faces.size();

   if ( verticesCount == 0 || trianglesCount == 0 )
   {
      delete vertices;
      return;
   }

   ID3DXMesh* dxMesh = NULL;
   DWORD FVF = vertices->getFVF();
   HRESULT res = D3DXCreateMeshFVF( trianglesCount, verticesCount, D3DXMESH_MANAGED, FVF, m_d3Device, &dxMesh );
   if ( FAILED(res) ) 
   { 
      ASSERT_MSG( false, "Can't create a mesh" ); 
      return;
   }

   // fill the vertex buffer, analyze the bounding sphere radius on the way
   void* pVertex = NULL;
   res = dxMesh->LockVertexBuffer( 0, (void**)&pVertex );
   if ( FAILED( res ) ) 
   { 
      ASSERT_MSG( false, "Can't lock the mesh's vertex buffer" );
      return;
   }
   vertices->copyTo( pVertex );
   dxMesh->UnlockVertexBuffer();

   // fill the index buffer & the attributes table
   USHORT* pIndex = NULL;
   DWORD* pAttrib = NULL;
   res = dxMesh->LockIndexBuffer( 0, (void**)&pIndex );
   if ( FAILED( res ) ) 
   { 
      ASSERT_MSG( false, "Can't lock the mesh's index buffer" ); 
      return;
   }
   res = dxMesh->LockAttributeBuffer(0, &pAttrib );
   if ( FAILED( res ) ) 
   { 
      ASSERT_MSG( false, "Can't lock the mesh's attributes buffer" );
      return;
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
  
   delete vertices;

   // copy the buffers
   dxMesh->GetVertexBuffer( &m_vb );
   dxMesh->GetIndexBuffer( &m_ib );
   m_verticesCount = dxMesh->GetNumVertices();
   m_facesCount = dxMesh->GetNumFaces();
   dxMesh->Release();
}

///////////////////////////////////////////////////////////////////////////////
