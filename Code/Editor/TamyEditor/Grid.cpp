#include "Grid.h"
#include "GridRenderingEffect.h"
#include "core-Renderer.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT(Grid, Entity)
END_OBJECT()

///////////////////////////////////////////////////////////////////////////////

Grid::Grid()
{
}

///////////////////////////////////////////////////////////////////////////////

Grid::Grid( Renderer& renderer, ResourcesManager& rm )
{
   LineSegments* gridLines = new LineSegments();
   renderer.implement< LineSegments >( *gridLines );

   std::string gridShaderResourceName = "Editor/Shaders/GridRenderingEffect.tfx";
   Shader* shader = NULL;
   if ( ( shader = dynamic_cast< Shader* >( rm.findResource( gridShaderResourceName ) ) ) == NULL )
   {
      std::string gridShaderFilename = "Editor/Shaders/GridRenderingEffect.fx";
      shader = new Shader( gridShaderFilename );
      rm.addResource( shader );
   }
   ShaderEffect* effect = new GridRenderingEffect();
   effect->initialize( *shader );

   Renderable* renderable = new Renderable();
   renderable->add( new Geometry( *gridLines ) );
   renderable->add(effect); 
   this->add(renderable);

   float dim = 100.0f;
   float varPos;
   for (int i = -25; i <= 25; ++i)
   {
      varPos = i * 4.0f;
      gridLines->add(LineSegment(D3DXVECTOR3(-dim, 0, varPos), D3DXVECTOR3(dim, 0, varPos)));
      gridLines->add(LineSegment(D3DXVECTOR3(varPos, 0, -dim), D3DXVECTOR3(varPos, 0, dim)));
   }
   gridLines->rebuild();
}

///////////////////////////////////////////////////////////////////////////////
