#include "dx9-Renderer\DX9RenderingPipelineMechanism.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "dx9-Renderer\DXErrorParser.h"
#include "core\Color.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9RenderingPipelineMechanism::DX9RenderingPipelineMechanism( RenderingPipelineMechanism& renderingPipelineMechanism )
   : m_renderingPipelineMechanism( renderingPipelineMechanism )
   , m_d3Device( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

DX9RenderingPipelineMechanism::~DX9RenderingPipelineMechanism()
{
   m_d3Device = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::initialize(Renderer& renderer)
{
   DX9Renderer* dxRenderer = dynamic_cast< DX9Renderer* >( &renderer );
   if ( dxRenderer == NULL )
   {
      throw std::runtime_error( "This implementation can work only with DX9Renderer" );
   }

   m_d3Device = &dxRenderer->getD3Device();
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::passBegin()
{
   if ( m_d3Device )
   {
      m_d3Device->BeginScene();
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9RenderingPipelineMechanism::passEnd()
{
   if ( m_d3Device )
   {
      m_d3Device->EndScene();
   }
}

///////////////////////////////////////////////////////////////////////////////
