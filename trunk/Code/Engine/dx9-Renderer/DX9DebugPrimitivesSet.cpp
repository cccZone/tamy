#include "dx9-Renderer/DX9DebugPrimitivesSet.h"
#include "dx9-Renderer/DX9Renderer.h"


/////////////////////////////////////////////////////////////////////////////

DX9RegularDebugPrimitivesSet::DX9RegularDebugPrimitivesSet( DX9Renderer& renderer, D3DPRIMITIVETYPE primitiveType, bool useZBuffer, uint maxPrimitives )
   : m_primitiveType( primitiveType )
   , m_useZBuffer( useZBuffer )
   , m_maxVertices( 0 )
   , m_buffer( NULL )
   , m_pVertices( NULL )
   , m_primitivesCount( 0 )
{
   switch( m_primitiveType )
   {
   case D3DPT_LINELIST:
      {
         m_maxVertices = maxPrimitives * 2;
         break;
      }
   case D3DPT_TRIANGLELIST:
      {
         m_maxVertices = maxPrimitives * 3;
         break;
      }
   default:
      ASSERT_MSG( false, "Unhandled primitive type" );
   }

   m_buffer = renderer.createVertexBuffer( m_maxVertices * sizeof( DebugVertex ), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED );
}

/////////////////////////////////////////////////////////////////////////////

DX9RegularDebugPrimitivesSet::~DX9RegularDebugPrimitivesSet()
{
   if ( m_buffer )
   {
      m_buffer->Release();
      m_buffer = NULL;
   }
   m_pVertices = NULL;
}

/////////////////////////////////////////////////////////////////////////////

void DX9RegularDebugPrimitivesSet::begin()
{
   m_primitivesCount = 0;
   ASSERT_MSG( m_pVertices == NULL, "Debug lines VB wasn't properly unlocked" );
   m_buffer->Lock( 0, m_maxVertices * sizeof( DebugVertex ), (void**)&m_pVertices, 0 );
}

/////////////////////////////////////////////////////////////////////////////

void DX9RegularDebugPrimitivesSet::end()
{
   if ( m_buffer != NULL )
   {
      m_buffer->Unlock();
      m_pVertices = NULL;
   }
}

/////////////////////////////////////////////////////////////////////////////

void DX9RegularDebugPrimitivesSet::draw( IDirect3DDevice9* d3Device )
{
   ASSERT_MSG( m_buffer != NULL, "No vertex buffer created" );

   if ( m_primitivesCount > 0 )
   {
      m_buffer->Unlock();
      m_pVertices = NULL;

      d3Device->SetRenderState( D3DRS_ZENABLE, m_useZBuffer );
      d3Device->SetRenderState( D3DRS_ZWRITEENABLE, m_useZBuffer );

      d3Device->SetStreamSource( 0, m_buffer, 0, sizeof( DebugVertex ) );
      d3Device->DrawPrimitive( m_primitiveType, 0, m_primitivesCount );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

DX9IndexedDebugPrimitivesSet::DX9IndexedDebugPrimitivesSet( DX9Renderer& renderer, D3DPRIMITIVETYPE primitiveType, bool useZBuffer, uint maxPrimitives )
   : m_primitiveType( primitiveType )
   , m_useZBuffer( useZBuffer )
   , m_maxVertices( 0 )
   , m_maxIndices( 0 )
   , m_idxBuffer( NULL )
   , m_vtxBuffer( NULL )
   , m_pVertices( NULL )
   , m_pIndices( NULL )
   , m_verticesCount( 0 )
   , m_indicesCount( 0 )
   , MAX_MESHES( 512 )
   , m_meshesCount( 0 )
{
   uint primitivesCount = 0;
   switch( m_primitiveType )
   {
   case D3DPT_LINELIST:
      {
         m_maxVertices = maxPrimitives * 2;
         m_maxIndices = maxPrimitives;
         break;
      }
   case D3DPT_TRIANGLELIST:
      {
         m_maxVertices = maxPrimitives * 3;
         m_maxIndices = maxPrimitives;
         break;
      }
   default:
      ASSERT_MSG( false, "Unhandled primitive type" );
   }

   m_vtxBuffer = renderer.createVertexBuffer( m_maxVertices * sizeof( DebugVertex ), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_MANAGED );
   m_idxBuffer = renderer.createIndexBuffer( m_maxIndices * sizeof( word ), 0, D3DFMT_INDEX16, D3DPOOL_MANAGED );

   m_meshes.resize( MAX_MESHES );
   for ( uint i = 0; i < MAX_MESHES; ++i )
   {
      m_meshes[i] = new Mesh();
   }
}

/////////////////////////////////////////////////////////////////////////////

DX9IndexedDebugPrimitivesSet::~DX9IndexedDebugPrimitivesSet()
{
   if ( m_vtxBuffer )
   {
      m_vtxBuffer->Release();
      m_vtxBuffer = NULL;
   }
   m_pVertices = NULL;

   if ( m_idxBuffer )
   {
      m_idxBuffer->Release();
      m_idxBuffer = NULL;
   }
   m_pIndices = NULL;

   for ( uint i = 0; i < MAX_MESHES; ++i )
   {
      delete m_meshes[i];
   }
   m_meshes.clear();
}

/////////////////////////////////////////////////////////////////////////////

void DX9IndexedDebugPrimitivesSet::addMesh( uint indicesCount )
{
   m_meshes[m_meshesCount]->m_firstVertexIdx = m_verticesCount;
   m_meshes[m_meshesCount]->m_firstIndexIdx = m_indicesCount;
   m_meshes[m_meshesCount]->m_indicesCount = indicesCount;
   ++m_meshesCount;
}

/////////////////////////////////////////////////////////////////////////////

void DX9IndexedDebugPrimitivesSet::begin()
{
   m_verticesCount = 0;
   m_indicesCount = 0;
   m_meshesCount = 0;

   ASSERT_MSG( m_pVertices == NULL, "Debug meshes VB wasn't properly unlocked" );
   m_vtxBuffer->Lock( 0, m_maxVertices * sizeof( DebugVertex ), (void**)&m_pVertices, 0 );

   ASSERT_MSG( m_pIndices == NULL, "Debug meshes IB wasn't properly unlocked" );
   m_idxBuffer->Lock( 0, m_maxIndices * sizeof( word ), (void**)&m_pIndices, 0 );

}

/////////////////////////////////////////////////////////////////////////////

void DX9IndexedDebugPrimitivesSet::end()
{
   if ( m_vtxBuffer != NULL )
   {
      m_vtxBuffer->Unlock();
      m_pVertices = NULL;
   }

   if ( m_idxBuffer != NULL )
   {
      m_idxBuffer->Unlock();
      m_pIndices = NULL;
   }
}

/////////////////////////////////////////////////////////////////////////////

void DX9IndexedDebugPrimitivesSet::draw( IDirect3DDevice9* d3Device )
{
   ASSERT_MSG( m_vtxBuffer != NULL || m_idxBuffer != NULL , "No vertex or index buffer created" );

   if ( m_meshesCount > 0 )
   {
      m_vtxBuffer->Unlock();
      m_pVertices = NULL;

      m_idxBuffer->Unlock();
      m_pIndices = NULL;

      d3Device->SetRenderState( D3DRS_ZENABLE, m_useZBuffer );
      d3Device->SetRenderState( D3DRS_ZWRITEENABLE, m_useZBuffer );

      d3Device->SetStreamSource( 0, m_vtxBuffer, 0, sizeof( DebugVertex ) );
      d3Device->SetIndices( m_idxBuffer );

      for ( word i = 0; i < m_meshesCount; ++i )
      {
         uint primitivesCount = 0;
         switch( m_primitiveType )
         {
         case D3DPT_LINELIST:
            {
               primitivesCount = m_meshes[i]->m_indicesCount / 2;
               break;
            }
         case D3DPT_TRIANGLELIST:
            {
               primitivesCount = m_meshes[i]->m_indicesCount / 3;
               break;
            }
         default:
            ASSERT_MSG( false, "Unhandled primitive type" );
         }      
         d3Device->DrawIndexedPrimitive( m_primitiveType, m_meshes[i]->m_firstVertexIdx, 0, m_verticesCount, m_meshes[i]->m_firstIndexIdx, primitivesCount );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
