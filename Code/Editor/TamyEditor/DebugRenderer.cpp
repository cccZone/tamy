#include "DebugRenderer.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include <stdexcept>

// effects
#include "GridRenderingEffect.h"


// TODO: dodac full pro API do rysowania debug'owych informacji

///////////////////////////////////////////////////////////////////////////////

DebugRenderer::DebugRenderer( ResourcesManager& resMgr, Camera& camera )
: m_renderer( NULL )
, m_resMgr( resMgr )
, m_camera( camera )
, m_localModel( new Model() )
, m_renderingPass( NULL )
{
   // add a camera to the scene
   m_localModel->addComponent( new ModelComponent< Camera >( m_camera ) );
}

///////////////////////////////////////////////////////////////////////////////

DebugRenderer::~DebugRenderer()
{
   delete m_renderingPass;
   m_renderingPass = NULL;

   delete m_localModel;
   m_localModel = NULL;

   m_entities.clear();
   m_freeHandles.clear();
}

///////////////////////////////////////////////////////////////////////////////

DebugHandle DebugRenderer::drawMesh( Geometry* geometry )
{
   // TODO: create a renderable with this geometry and add it using 'drawEntity' method
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

DebugHandle DebugRenderer::drawEntity( SpatialEntity* entity )
{
   if ( !entity )
   {
      throw std::invalid_argument( "NULL pointer instead a SpatialEntity instance" );
   }

   // find a handle for out entity
   DebugHandle handle;
   if ( !m_freeHandles.empty() )
   {
      handle = m_freeHandles.back();
      m_freeHandles.pop_back();
   }
   else
   {
      handle = m_entities.size();
      m_entities.push_back( NULL );
   }

   // add the entity to the entities array at the location the handle specifies
   m_entities[ handle ] = entity;

   // add the entity to the model
   m_localModel->add( entity );

   return handle;
}

///////////////////////////////////////////////////////////////////////////////

void DebugRenderer::stopDrawing( DebugHandle handle )
{
   SpatialEntity* entity = m_entities.at( handle );
   m_entities[ handle ] = NULL;
   m_freeHandles.push_back( handle );

   m_localModel->remove( *entity );
}

///////////////////////////////////////////////////////////////////////////////

void DebugRenderer::initialize( Renderer& renderer )
{
   m_renderer = &renderer;

   // create a reference grid
   m_localModel->add( new GridRenderingEffect( createGrid( m_renderer ), m_resMgr, m_camera ) );

   delete m_renderingPass;
   m_renderingPass = new SceneRenderingPass();
   m_renderingPass->addScene( *m_localModel );
   m_renderingPass->initialize( renderer );
}

///////////////////////////////////////////////////////////////////////////////

LineSegments* DebugRenderer::createGrid( Renderer* renderer ) const
{
   LineSegments* gridLines = new LineSegments();
   renderer->implement< LineSegments >( *gridLines );

   float dim = 100.0f;
   float varPos;
   for (int i = -25; i <= 25; ++i)
   {
      varPos = i * 4.0f;
      gridLines->add(LineSegment(D3DXVECTOR3(-dim, 0, varPos), D3DXVECTOR3(dim, 0, varPos)));
      gridLines->add(LineSegment(D3DXVECTOR3(varPos, 0, -dim), D3DXVECTOR3(varPos, 0, dim)));
   }
   gridLines->rebuild();

   return gridLines;
}

///////////////////////////////////////////////////////////////////////////////

void DebugRenderer::render()
{
   if ( m_renderer && m_renderingPass )
   {
      m_renderer->setRenderTarget( NULL );
      m_renderingPass->render();
   }
}

///////////////////////////////////////////////////////////////////////////////
