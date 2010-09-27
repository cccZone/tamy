#include "dx9-Renderer/DX9Skeleton.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core\File.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9Skeleton::DX9Skeleton( Skeleton& skeleton )
   : m_skeleton( skeleton )
   , m_d3Device( NULL )
   , m_vb( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9Skeleton::~DX9Skeleton()
{
   m_d3Device = NULL;

   if ( m_vb )
   {
      m_vb->Release();
      m_vb = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9Skeleton::initialize( Renderer& renderer )
{
   DX9Renderer* d3dRenderer = dynamic_cast< DX9Renderer* >( &renderer );
   if ( d3dRenderer == NULL )
   {
      throw std::runtime_error( "This class works only with DX9Renderer instance" );
   }

   m_d3Device = &d3dRenderer->getD3Device();
  
   const std::vector< VertexWeight >& weights = m_skeleton.getVertexWeights();
   unsigned int weightsCount = weights.size();
   m_vb = d3dRenderer->createVertexBuffer( weightsCount * sizeof( VertexWeight ), 0, VertexWeight::FVF, D3DPOOL_MANAGED );

   VertexWeight* pWeight = NULL;
   HRESULT res = m_vb->Lock( 0, 0, (void**)&pWeight, 0 );
   if ( FAILED( res ) ) 
   { 
      throw std::runtime_error( "Can't lock a vertex buffer" );
   }

   for ( unsigned int i = 0; i < weightsCount; ++i )
   {
      *pWeight = weights[i];
      pWeight++;
   }

   res = m_vb->Unlock();
   if ( FAILED( res ) ) 
   { 
      throw std::runtime_error("Failed to unlock the vertex buffer");
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9Skeleton::setInStream()
{
   if ( m_d3Device && m_vb )
   {
      m_d3Device->SetStreamSource( 1, m_vb, 0, sizeof( VertexWeight ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
