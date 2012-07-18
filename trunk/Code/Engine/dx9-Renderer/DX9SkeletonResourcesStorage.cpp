#include "dx9-Renderer\DX9SkeletonResourcesStorage.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

template<>
IDirect3DVertexBuffer9* RenderResourceStorage< DX9Renderer, Skeleton, IDirect3DVertexBuffer9 >::createResource( const Skeleton& obj ) const
{
   const std::vector< VertexWeight >& weights = obj.getVertexWeights();
   unsigned int weightsCount = weights.size();
   IDirect3DVertexBuffer9* vb = m_renderer.createVertexBuffer( weightsCount * sizeof( VertexWeight ), 0, VertexWeight::FVF, D3DPOOL_MANAGED );

   VertexWeight* pWeight = NULL;
   HRESULT res = vb->Lock( 0, 0, (void**)&pWeight, 0 );
   if ( FAILED( res ) ) 
   { 
      ASSERT_MSG( false, "Can't lock a vertex buffer" );
      return NULL;
   }

   for ( unsigned int i = 0; i < weightsCount; ++i )
   {
      *pWeight = weights[i];
      pWeight++;
   }

   res = vb->Unlock();
   if ( FAILED( res ) ) 
   { 
      ASSERT_MSG( false, "Failed to unlock the vertex buffer");
      return NULL;
   }

   return vb;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Skeleton, IDirect3DVertexBuffer9 >::releaseResource( IDirect3DVertexBuffer9* resource ) const
{
   if ( resource )
   {
      resource->Release();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Skeleton, IDirect3DVertexBuffer9 >::deviceLost( IDirect3DVertexBuffer9* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Skeleton, IDirect3DVertexBuffer9 >::deviceRestored( IDirect3DVertexBuffer9* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////
