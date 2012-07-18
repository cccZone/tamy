#include "dx9-Renderer\DX9EffectsResourcesStorage.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

template<>
ID3DXEffect* RenderResourceStorage< DX9Renderer, EffectShader, ID3DXEffect >::createResource( const EffectShader& obj ) const
{
   const std::string& effectContents = obj.getScript();

   ID3DXEffect* effect = NULL;
   ID3DXBuffer* errorsBuf = NULL;

   IDirect3DDevice9* d3Device = &m_renderer.getD3Device();
   HRESULT res = D3DXCreateEffect( d3Device, effectContents.c_str(), effectContents.length(), NULL, NULL, 0, NULL, &effect, &errorsBuf );

   if ( FAILED( res ) || ( effect == NULL ) )
   {
      if ( errorsBuf != NULL )
      {
         std::string compilationErrors = ( const char* )errorsBuf->GetBufferPointer();
         errorsBuf->Release();
         std::string errMsg =std::string( "Effect compilation error: " ) + compilationErrors;
         ASSERT_MSG( false, errMsg.c_str() );
      }
      else
      {
         std::string errMsg = translateDxError( "Error while loading an effect", res );
         ASSERT_MSG( false, errMsg.c_str() );
      }
   }

   return effect;
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, EffectShader, ID3DXEffect >::releaseResource( ID3DXEffect* resource ) const
{
   if( resource )
   {
      resource->Release();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, EffectShader, ID3DXEffect >::deviceLost( ID3DXEffect* resource ) const
{
   if( resource )
   {
      resource->OnLostDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////

template<>
void RenderResourceStorage< DX9Renderer, EffectShader, ID3DXEffect >::deviceRestored( ID3DXEffect* resource ) const
{
   if( resource )
   {
      resource->OnResetDevice();
   }
}

///////////////////////////////////////////////////////////////////////////////
