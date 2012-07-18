#include "dx9-Renderer\DX9FontResourcesStorage.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

template<>
ID3DXFont* RenderResourceStorage< DX9Renderer, Font, ID3DXFont >::createResource( const Font& obj ) const
{
   /*// create the font
   ID3DXFont* dxFont = NULL;
   IDirect3DDevice9* d3Device = &m_renderer.getD3Device();
   HRESULT res = D3DXCreateFontIndirect( d3Device,  &obj.getDescription(), &dxFont );

   if ( FAILED( res ) || dxFont == NULL )
   {
      ASSERT_MSG( false,  "Font could not be created" );
      return NULL;
   }
   */
   // <renderer.todo> implement fonts

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Font, ID3DXFont >::releaseResource( ID3DXFont* resource ) const
{
   if( resource )
   {
      resource->Release();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Font, ID3DXFont >::deviceLost( ID3DXFont* resource ) const
{
   if( resource )
   {
      resource->OnLostDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, Font, ID3DXFont >::deviceRestored( ID3DXFont* resource ) const
{
   if( resource )
   {
      resource->OnResetDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////
