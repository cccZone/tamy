#include "DebugRenderer.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include <stdexcept>

// effects
#include "GridRenderingEffect.h"


///////////////////////////////////////////////////////////////////////////////

DebugRenderer::DebugRenderer( Renderer& renderer, Camera& camera )
: m_renderer( renderer )
, m_camera( camera )
, m_localModel( new Model() )
{
   // create a reference grid
   m_gridLines = createGrid();

   Renderable* grid = new Renderable();
   grid->add( new Geometry( *m_gridLines ) );
   grid->add( new GridRenderingEffect( m_renderer ) ); 
   m_localModel->add( grid );

   // add a camera to the scene
   m_localModel->addComponent( new CameraComponent( m_camera ) );
}

///////////////////////////////////////////////////////////////////////////////

DebugRenderer::~DebugRenderer()
{
   delete m_gridLines;
   m_gridLines = NULL;

   delete m_localModel;
   m_localModel = NULL;

   m_entities.clear();
   m_freeHandles.clear();
}

///////////////////////////////////////////////////////////////////////////////

LineSegments* DebugRenderer::createGrid() const
{
   LineSegments* gridLines = new LineSegments();
   m_renderer.implement< LineSegments >( *gridLines );

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

