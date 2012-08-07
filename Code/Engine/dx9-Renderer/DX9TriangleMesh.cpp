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
   m_verticesCount = vertices->size();
   m_facesCount = faces.size();

   if ( m_verticesCount == 0 || m_facesCount == 0 )
   {
      delete vertices;
      return;
   }

   // create a vertex buffer
   m_vb = m_renderer.createVertexBuffer( m_verticesCount * sizeof( LitVertex ), 0, 0, D3DPOOL_MANAGED );
   if ( !m_vb )
   {
      ASSERT_MSG( false, "Can't create a vertex buffer for the triangle mesh" ); 
      return;
   }

   // create an index buffer
   m_ib = m_renderer.createIndexBuffer( m_facesCount * sizeof( Face ), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED );
   if ( !m_ib )
   {
      ASSERT_MSG( false, "Can't create an index buffer for the triangle mesh" ); 
      return;
   }

   // fill the vertex buffer
   {
      void* pVertex = NULL;
      HRESULT res = m_vb->Lock( 0, 0, (void**)&pVertex, 0 );
      if ( FAILED( res ) ) 
      { 
         ASSERT_MSG( false, "Can't lock the mesh's vertex buffer" );
         return;
      }
      vertices->copyTo( pVertex );
      m_vb->Unlock();
   }

   // fill the index buffer
   {
      USHORT* pIndex= NULL;
      HRESULT res = m_ib->Lock( 0, 0, (void**)&pIndex, 0 );
      if ( FAILED( res ) ) 
      { 
         ASSERT_MSG( false, "Can't lock the mesh's index buffer" );
         return;
      }
      
      for ( unsigned int i = 0; i < m_facesCount; ++i )
      {
         const Face& face = faces[i];
         *pIndex++  = face.idx[0]; 
         *pIndex++  = face.idx[1]; 
         *pIndex++  = face.idx[2];
      }

      m_ib->Unlock();
   }


   /*

   ID3DXMesh* dxMesh = NULL;
   HRESULT res = D3DXCreateMeshFVF( trianglesCount, verticesCount, D3DXMESH_MANAGED, 0, m_d3Device, &dxMesh );
   if ( FAILED(res) ) 
   { 
      ASSERT_MSG( false, "Can't create a vertex buffer for the triangle mesh" ); 
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
   */
}

///////////////////////////////////////////////////////////////////////////////
