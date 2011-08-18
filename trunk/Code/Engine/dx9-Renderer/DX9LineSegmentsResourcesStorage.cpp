#include "dx9-Renderer\DX9LineSegmentsResourcesStorage.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template<>
DX9LineSegments* RenderResourceStorage< DX9Renderer, LineSegments, DX9LineSegments >::createResource( const LineSegments& obj ) const
{
   return new DX9LineSegments( m_renderer, obj );
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, LineSegments, DX9LineSegments >::releaseResource( DX9LineSegments* resource ) const
{
   delete resource;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, LineSegments, DX9LineSegments >::deviceLost( DX9LineSegments* resource ) const
{
   if( resource )
   {
      resource->onLostDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, LineSegments, DX9LineSegments >::deviceRestored( DX9LineSegments* resource ) const
{
   if( resource )
   {
      resource->onResetDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////
