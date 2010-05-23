#include "DebugRenderer.h"
#include "core-MVC.h"
#include "core-Renderer.h"

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

DebugHandle DebugRenderer::drawMesh( Geometry& geometry )
{
   return 0;
}

///////////////////////////////////////////////////////////////////////////////

void DebugRenderer::stopDrawing( DebugHandle handle )
{
}

///////////////////////////////////////////////////////////////////////////////
