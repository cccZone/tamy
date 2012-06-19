#include "DebugGrid.h"
#include "core-Renderer/LineSegments.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/BasicRenderCommands.h"
#include "core-Renderer/LitVertex.h"
#include "DebugGeometryBuilder.h"
#include "SingleColorDebugMat.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ABSTRACT_OBJECT(DebugGrid)
   PARENT( Geometry )
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

DebugGrid::DebugGrid()
{
   Color gridColor( 100, 100, 255 );
   // setup the material
   {
      m_material = new SingleColorDebugMat( gridColor );
      addState( *m_material );
   }

   // setup grid geometry
   {   
      m_mesh = DebugGeometryBuilder::createGrid( 1000, 50, gridColor );
      setMesh( *m_mesh );
   }
}

///////////////////////////////////////////////////////////////////////////////

DebugGrid::~DebugGrid()
{
   delete m_material;
   m_material = NULL;

   delete m_mesh;
   m_mesh = NULL;
}

///////////////////////////////////////////////////////////////////////////////

bool DebugGrid::onPreRender( Renderer& renderer )
{
   new ( renderer() ) RCSetVertexDeclaration( LitVertex::FVF );
   new ( renderer() ) RCSetWorldMatrix( Matrix::IDENTITY );

   return true;
}

///////////////////////////////////////////////////////////////////////////////
