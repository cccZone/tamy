#include "core-Renderer/RPSBComputed.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Defines.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/RPSceneRenderNode.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/ComputedRenderState.h"
#include "core-Renderer/Geometry.h"



///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPSBComputed );
   PARENT( RPSceneBuilder );
   PROPERTY( RPSceneRenderNode*, m_hostNode );
   PROPERTY_EDIT( "Compute pixel shader", PixelShader*, m_shader );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPSBComputed::RPSBComputed()
   : m_shader( NULL )
   , m_shaderNode( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

RPSBComputed::~RPSBComputed()
{
   delete m_shaderNode; m_shaderNode = NULL;
   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPSBComputed::onDefineSockets( RPSceneRenderNode& hostNode )
{
   m_hostNode = &hostNode;

   delete m_shaderNode;
   m_shaderNode = new PixelShaderNodeOperator< RenderingPipelineNode >( hostNode );
   if ( m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSBComputed::onObjectLoaded()
{
   __super::onObjectLoaded();

   ASSERT( m_hostNode != NULL );
   ASSERT( m_shaderNode == NULL );
   m_shaderNode = new PixelShaderNodeOperator< RenderingPipelineNode >( *m_hostNode );
   if ( m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSBComputed::onPrePropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shaderNode )
   {
      m_shaderNode->resetShader();
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPSBComputed::onPropertyChanged( ReflectionProperty& property )
{
   __super::onPropertyChanged( property );

   if ( property.getName() == "m_shader" && m_shaderNode && m_shader )
   {
      m_shaderNode->setShader( *m_shader );
   }
}

///////////////////////////////////////////////////////////////////////////////

StateTreeNode* RPSBComputed::buildRenderTree( MemoryPool& pool, const Array< Geometry* >& visibleElems, RuntimeDataBuffer& data ) const
{
   if ( !m_shader || !m_shaderNode )
   {
      return NULL;
   }

   // ignore all render states - just gather the geometry under one state node that will
   // set our pixel shader that will render the depth and normals

   ComputedRenderState* renderState = new ( pool ) ComputedRenderState( *m_shader, *m_shaderNode, data );
   StateTreeNode* root = new ( pool ) StateTreeNode( renderState, renderState );
   GeometryNode** currGeometryNode = &(root->m_geometryNode);

   unsigned int elemsCount = visibleElems.size();
   for ( unsigned int i = 0; i < elemsCount; ++i )
   {     
      Geometry* geometry = visibleElems[i];
      *currGeometryNode = new ( pool ) GeometryNode( *currGeometryNode, *geometry );
   }

   return root;
}

///////////////////////////////////////////////////////////////////////////////
