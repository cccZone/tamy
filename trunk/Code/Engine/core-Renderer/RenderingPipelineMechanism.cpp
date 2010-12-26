#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipeline.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/DefaultAttributeSorter.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/RenderTargetDescriptor.h"
#include "core-Renderer/SpatialView.h"
#include "core-Renderer/RenderingView.h"
#include "core/AABoundingBox.h"
#include "core-MVC/Model.h"
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

BEGIN_RTTI( RenderingPipelineMechanism )
END_RTTI

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::RenderingPipelineMechanism( RenderingPipeline* pipeline )
   : m_pipeline( pipeline )
   , m_cameraContext( NULL )
   , m_spatialView( NULL )
   , m_renderingView( NULL )
   , m_statesManager( new DefaultAttributeSorter() )
   , m_renderer( NULL )
{
   // create the model views
   AABoundingBox sceneBB(D3DXVECTOR3( -10000, -10000, -10000 ), D3DXVECTOR3( 10000, 10000, 10000 ) );
   m_spatialView = new SpatialView( sceneBB );

   m_renderingView = new RenderingView();
   m_renderingView->setAttributeSorter( *m_statesManager );
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineMechanism::~RenderingPipelineMechanism()
{
   m_renderer = NULL;

   if ( m_pipeline )
   {
      m_pipeline->deinitialize( *this );
   }

   delete m_cameraContext;
   m_cameraContext = NULL;

   delete m_statesManager;
   m_statesManager = NULL;

   delete m_spatialView;
   m_spatialView = NULL;

   delete m_renderingView;
   m_renderingView = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::addScene( Model& scene )
{
   // check if the scene is there, and if it is - don't add it twice
   std::vector< Model* >::const_iterator it = std::find( m_scenes.begin(), m_scenes.end(), &scene );
   if ( it == m_scenes.end() )
   {
      m_scenes.push_back( &scene );
   }

   // notify the pipeline that the runtime params have changed
   scene.attach( *m_spatialView );
   scene.attach( *m_renderingView );
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::removeScene( Model& scene )
{
   scene.detach( *m_spatialView );
   scene.detach( *m_renderingView );

   std::vector< Model* >::const_iterator it = std::find( m_scenes.begin(), m_scenes.end(), &scene );
   if ( it != m_scenes.end() )
   {
      m_scenes.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::setCamera( Camera& camera )
{
   delete m_cameraContext;
   m_cameraContext = new PlainCameraContext( camera );
}

///////////////////////////////////////////////////////////////////////////////

RenderTarget& RenderingPipelineMechanism::getRenderTarget( const std::string& id ) const
{
   if ( m_pipeline )
   {
      return m_pipeline->getRenderTarget( id );
   }
   else
   {
      throw std::runtime_error( "Rendering pipeline mechanism isn't properly initialized" );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::initialize( Renderer& renderer )
{
   // implement the pass
   m_renderer = &renderer;
   renderer.implement< RenderingPipelineMechanism >( *this );

   if ( m_pipeline )
   {
      m_pipeline->initialize( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineMechanism::render()
{
   if ( !m_renderer || !m_pipeline || !m_cameraContext )
   {
      return;
   }

   // run through the scene visibility
   m_spatialView->update( *m_cameraContext );

   // render the pipeline
   impl().passBegin();
   m_pipeline->render( *this );
   impl().passEnd();
}

///////////////////////////////////////////////////////////////////////////////
