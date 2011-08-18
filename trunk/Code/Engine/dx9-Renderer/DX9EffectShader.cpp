#include "core-Renderer\EffectShader.h"
#include "dx9-Renderer\DX9Renderer.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

void RCBindEffect::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );

   ID3DXEffect* dxEffect = dxRenderer.getEffect( m_shader );
   if ( !dxEffect )
   {
      return;
   }

   // set the shader parameters
   dxEffect->SetTechnique( m_techniqueName.c_str() );
   setParams( renderer, dxEffect );

   // begin the rendering process
   unsigned int passesCount;
   dxEffect->Begin( &passesCount, 0 );

   // TODO: implement the multi-pass support in the renderer
   // dxEffect->BeginPass(passIdx); dxEffect->EndPass();
}

///////////////////////////////////////////////////////////////////////////////

void RCUnbindEffect::execute( Renderer& renderer )
{
   DX9Renderer& dxRenderer = static_cast< DX9Renderer& >( renderer );

   ID3DXEffect* dxEffect = dxRenderer.getEffect( m_shader );
   if ( dxEffect )
   {
      dxEffect->End();
   }
}

///////////////////////////////////////////////////////////////////////////////
