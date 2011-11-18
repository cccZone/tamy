#include "core-Renderer/RPSBComputed.h"
#include "core-Renderer/SpatialRepresentation.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Defines.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Camera.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSBComputed, RPSceneBuilder );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSBComputed::RPSBComputed()
   : m_shader( NULL )
{
   ResourcesManager& mgr = ResourcesManager::getInstance();

   Resource& psRes = mgr.create( SHADERS_DIR "RenderingPipeline/DepthNormals.tpsh" );
   m_shader = DynamicCast< PixelShader >( &psRes );
}

///////////////////////////////////////////////////////////////////////////////

RPSBComputed::~RPSBComputed()
{
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPSBComputed::onPreRender( Renderer& renderer )
{
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

   Camera& activeCam = renderer.getActiveCamera();
   comm->setFloat( "g_FarZ", activeCam.getFarClippingPlane() );
}

///////////////////////////////////////////////////////////////////////////////

void RPSBComputed::onPostRender( Renderer& renderer )
{
   new ( renderer() ) RCUnbindPixelShader( *m_shader );
}

///////////////////////////////////////////////////////////////////////////////

StateTreeNode* RPSBComputed::buildRenderTree( MemoryPool& pool, const Array< SpatialRepresentation* >& visibleElems ) const
{
   // ignore all render states - just gather the geometry under one state node that will
   // set our pixel shader that will render the depth and normals

   StateTreeNode* root = new ( pool ) StateTreeNode( const_cast< RPSBComputed* >( this ) );
   GeometryNode** currGeometryNode = &(root->m_geometryNode);

   unsigned int elemsCount = visibleElems.size();
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {     
      Geometry& geometry = visibleElems[i]->getGeometry();
      *currGeometryNode = new ( pool ) GeometryNode( *currGeometryNode, geometry );
   }

   return root;
}

///////////////////////////////////////////////////////////////////////////////
