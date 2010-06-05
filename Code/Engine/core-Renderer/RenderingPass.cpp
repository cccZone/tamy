#include "core-Renderer\RenderingPass.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\DefaultAttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI( RenderingPass )
END_RTTI

///////////////////////////////////////////////////////////////////////////////

RenderingPass::RenderingPass( RenderTarget* renderTarget )
: m_statesManager( new DefaultAttributeSorter() )
, m_renderTarget( renderTarget )
, m_renderer( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RenderingPass::~RenderingPass()
{
   delete m_statesManager;
   m_statesManager = NULL;

   m_renderTarget = NULL;
   m_renderer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPass::initialize( Renderer& renderer )
{
   // implement the pass
   renderer.implement< RenderingPass >( *this );

   m_renderer = &renderer;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPass::render()
{
   if ( !m_renderer || !onPreRender() )
   {
      return;
   }

   m_renderer->setRenderTarget( m_renderTarget );

   impl().passBegin();
   m_statesManager->render();
   impl().passEnd();

   onPostRender();
}

///////////////////////////////////////////////////////////////////////////////
