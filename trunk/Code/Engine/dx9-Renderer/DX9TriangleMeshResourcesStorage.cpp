#include "dx9-Renderer\DX9TriangleMeshResourcesStorage.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
DX9TriangleMesh* RenderResourceStorage< DX9Renderer, TriangleMesh, DX9TriangleMesh >::createResource( const TriangleMesh& obj ) const
{
   return new DX9TriangleMesh( m_renderer, obj );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, TriangleMesh, DX9TriangleMesh >::releaseResource( DX9TriangleMesh* resource ) const
{
   delete resource;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, TriangleMesh, DX9TriangleMesh >::deviceLost( DX9TriangleMesh* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, TriangleMesh, DX9TriangleMesh >::deviceRestored( DX9TriangleMesh* resource ) const
{
}

///////////////////////////////////////////////////////////////////////////////
