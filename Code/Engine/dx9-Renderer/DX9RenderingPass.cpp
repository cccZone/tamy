#include "dx9-Renderer\DX9RenderingPass.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\RenderTarget.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core\Color.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9RenderingPass::DX9RenderingPass( RenderingPass& renderingPass )
: m_renderingPass(renderingPass  )
, m_d3Device( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9RenderingPass::~DX9RenderingPass()
{
   m_d3Device = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPass::initialize(Renderer& renderer)
{
   DX9Renderer* dxRenderer = dynamic_cast< DX9Renderer* >( &renderer );
   if ( dxRenderer == NULL )
   {
      throw std::runtime_error( "This implementation can work only with DX9Renderer" );
   }

   m_d3Device = &dxRenderer->getD3Device();
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPass::passBegin()
{
   if ( m_d3Device )
   {
      m_d3Device->BeginScene();
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPass::passEnd()
{
   if ( m_d3Device )
   {
      m_d3Device->EndScene();
   }
}

///////////////////////////////////////////////////////////////////////////////
