#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\DepthBuffer.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Viewport.h"
#include "core\Point.h"
#include "core\Assert.h"
#include "core\MatrixUtils.h"
#include "core\Profiler.h"
#include "core\MemoryPoolAllocator.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class NullRenderingMechanism : public RenderingMechanism
   {
   public:
      void initialize( Renderer& renderer ) {}
      void deinitialize( Renderer& renderer ) {}
      void render( Renderer& renderer ) {}
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(unsigned int viewportWidth,
                   unsigned int viewportHeight)
: m_mechanism( new NullRenderingMechanism() )
, m_viewportWidth(viewportWidth)
, m_viewportHeight(viewportHeight)
, m_leftClientArea(0)
, m_topClientArea(0)
, m_rightClientArea(m_viewportWidth)
, m_bottomClientArea(m_viewportHeight)
, m_initialState(new InitialState())
, m_renderingState(new RenderingState())
, m_deviceLostState(new DeviceLostState())
, m_currentRendererState( m_initialState ) // we always start in the initial state
, m_renderCommands( new RoundBuffer( 1024 * 1024 ) ) // 1 MB for the commands
, m_vertexShaderTechnique( 0 )
, m_defaultDepthBuffer( NULL )
{
   m_defaultCamera = new Camera( "defaultCamera", *this, Camera::PT_PERSPECTIVE );
   m_camerasStack.push( m_defaultCamera );

   MatrixUtils::generateViewportMatrix( 0, 0, m_viewportWidth, m_viewportHeight, m_viewportMatrix );

   // create a memory pool dedicated to keeping the rendering commands related data
   m_commandsDataMemoryPool = new MemoryPool( 1024 * 1024 ); // 1MB for the commands data
   m_commandsDataAllocator = new MemoryPoolAllocator( m_commandsDataMemoryPool );
}

///////////////////////////////////////////////////////////////////////////////

Renderer::~Renderer()
{
   if ( m_mechanism )
   {
      m_mechanism->deinitialize( *this );
   }
   delete m_mechanism;
   m_mechanism = NULL;

   delete m_defaultCamera;
   m_defaultCamera = NULL;

   delete m_defaultDepthBuffer;
   m_defaultDepthBuffer = NULL;

   m_currentRendererState = NULL;

   delete m_initialState;
   m_initialState = NULL;

   delete m_renderingState;
   m_renderingState = NULL;

   delete m_deviceLostState;
   m_deviceLostState = NULL;

   delete m_commandsDataAllocator;
   m_commandsDataAllocator = NULL;

   delete m_commandsDataMemoryPool;
   m_commandsDataMemoryPool = NULL;

   delete m_renderCommands;
   m_renderCommands = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setMechanism( RenderingMechanism* mechanism )
{
   // deinitialize and release the old mechanism
   if ( m_mechanism )
   {
      m_mechanism->deinitialize( *this );
   }
   delete m_mechanism;

   // setup and initialize a new one
   if ( mechanism == NULL )
   {
      m_mechanism = new NullRenderingMechanism();
   }
   else
   {
      m_mechanism = mechanism;
   }
   m_mechanism->initialize( *this );
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::pushCamera( Camera& camera ) 
{ 
   m_camerasStack.push( &camera ); 
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::popCamera() 
{ 
   m_camerasStack.pop(); 

   if ( m_camerasStack.empty() ) 
   { 
      m_camerasStack.push( m_defaultCamera ); 
   } 
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::render()
{
   m_currentRendererState->render(*this);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::InitialState::render(Renderer& renderer)
{
   // create the default depth buffer
   delete renderer.m_defaultDepthBuffer;
   renderer.m_defaultDepthBuffer = new DepthBuffer( renderer.m_viewportWidth, renderer.m_viewportHeight );
   renderer.pushDepthBuffer( renderer.m_defaultDepthBuffer );

   renderer.resetViewport(renderer.m_viewportWidth, renderer.m_viewportHeight);
   renderer.setRenderingState();

   // we don't want to loose the rendering frame just for rendering - 
   // invoke the rendering method once again so that the actual rendering
   // takes place also
   renderer.render();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::RenderingState::render( Renderer& renderer )
{
   PROFILED();

   if ( renderer.isGraphicsSystemReady() == false )
   {
      renderer.setDeviceLostState();
      return;
   }

   renderer.resetRenderTargetsList();
   renderer.renderingBegin();
   renderer.m_mechanism->render( renderer );
   renderer.executeCommands();
   renderer.renderingEnd();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::DeviceLostState::render(Renderer& renderer)
{
   renderer.attemptToRecoverGraphicsSystem();

   if (renderer.isGraphicsSystemReady() == true) {renderer.setInitialState();}
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setInitialState()
{
   m_currentRendererState = m_initialState;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setRenderingState()
{
   m_currentRendererState = m_renderingState;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::setDeviceLostState()
{
   m_currentRendererState = m_deviceLostState;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::resizeViewport(unsigned int width, unsigned int height,
                              unsigned int leftClientArea, unsigned int topClientArea,
                              unsigned int rightClientArea, unsigned int bottomClientArea)
{
   m_viewportWidth = width;
   m_viewportHeight = height;
   MatrixUtils::generateViewportMatrix( 0, 0, m_viewportWidth, m_viewportHeight, m_viewportMatrix );

   m_leftClientArea = leftClientArea;
   m_topClientArea = topClientArea;
   m_rightClientArea = rightClientArea;
   m_bottomClientArea = bottomClientArea;

   // resize the default depth buffer as well
   if ( m_defaultDepthBuffer )
   {
      m_defaultDepthBuffer->setDimensions( width, height );
   }

   notify(RO_RESIZE_VIEWPORT);

   resizeViewport(m_viewportWidth, m_viewportHeight);
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::getViewport( Viewport& outViewportDesc )
{
   outViewportDesc.m_minZ = 0.0f;
   outViewportDesc.m_maxZ = 1.0f;
   outViewportDesc.m_offsetX = 0;
   outViewportDesc.m_offsetY = 0;
   outViewportDesc.m_width = m_viewportWidth;
   outViewportDesc.m_height = m_viewportHeight;
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::bindRenderTarget( RenderTarget* renderTarget, uint targetIdx )
{
   static Color backBufferColor( 0, 0, 0 );

   activateRenderTarget( renderTarget, targetIdx );
   if ( !shouldRenderTargetBeCleaned( renderTarget ) )
   {
      return;
   }

   if ( renderTarget )
   {
      // when rendering to a render target, we DON'T want the z-buffer cleaned ( at least not at this point )
      cleanRenderTarget( renderTarget->getBackgroundColor() );
   }
   else
   {
      // we only want the z-buffer cleaned when we're cleaning the back buffer
      cleanRenderTarget( backBufferColor );
   }
   markRenderTargetCleaned( renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::unbindRenderTarget( uint targetIdx )
{
   deactivateRenderTarget( targetIdx );
}

///////////////////////////////////////////////////////////////////////////////

bool Renderer::shouldRenderTargetBeCleaned( RenderTarget* renderTarget ) const
{
   return m_renderTargetsList.find( renderTarget ) == m_renderTargetsList.end();
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::markRenderTargetCleaned( RenderTarget* renderTarget )
{
   m_renderTargetsList.insert( renderTarget );
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::resetRenderTargetsList()
{
   m_renderTargetsList.clear();
}


/////////////////////////////////////////////////////////////////////////////

void Renderer::pushDepthBuffer( DepthBuffer* depthBuffer )
{
   if ( depthBuffer != NULL )
   {
      m_depthBuffersStack.push( depthBuffer );

      activateDepthBuffer( *depthBuffer );
   }
}

/////////////////////////////////////////////////////////////////////////////

void Renderer::popDepthBuffer( DepthBuffer* depthBuffer )
{
   ASSERT_MSG( m_depthBuffersStack.top() == depthBuffer, "Trying to deactivate a depth buffer that's not currently active" );
   if ( m_depthBuffersStack.top() != depthBuffer )
   {
      return;
   }

   m_depthBuffersStack.pop();
   ASSERT_MSG( !m_depthBuffersStack.empty(), "Somehow all depth buffers have been deactivated - check your code" );
   if ( m_depthBuffersStack.empty() )
   {
      // push back the default depth buffer - the stack MUSTN'T be empty
      m_depthBuffersStack.push( m_defaultDepthBuffer );
   }

   activateDepthBuffer( *m_depthBuffersStack.top() );
}

///////////////////////////////////////////////////////////////////////////////

void Renderer::executeCommands()
{
   RenderCommand* c = NULL;
   while ( c = m_renderCommands->front< RenderCommand >() )
   {
      c->execute( *this );
      delete c;
   }
}

///////////////////////////////////////////////////////////////////////////////
