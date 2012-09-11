#include "core-Renderer/RPPreviewNode.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/FullscreenQuad.h"
#include "core-Renderer/RenderTarget.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/Camera.h"
#include "core-Renderer/Defines.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_ENUM( PreviewType );
   ENUM_VAL( PT_Color );
   ENUM_VAL( PT_Depth );
   ENUM_VAL( PT_Normals );
   ENUM_VAL( PT_Specular );
END_ENUM();

///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPPreviewNode );
   PARENT( RenderingPipelineNode );
   PROPERTY_EDIT( "Data type", PreviewType, m_type );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPPreviewNode::RPPreviewNode()
   : m_type( PT_Color )
   , m_shader( NULL )
{
   m_texInput = new RPTextureInput( "Texture" );
   defineInput( m_texInput );

   // load the shader
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   FilePath shaderPath( DEDICATED_PIPELINE_SHADERS_DIR "preview.tpsh" );
   m_shader = resMgr.create< PixelShader >( shaderPath, true );
}

///////////////////////////////////////////////////////////////////////////////

void RPPreviewNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   // get the recreated inputs
   delete m_texInput;
   m_texInput = new RPTextureInput( "Texture" );
}

///////////////////////////////////////////////////////////////////////////////

void RPPreviewNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_shader )
   {
      return;
   }

   RuntimeDataBuffer& data = host.data();
   ShaderTexture* texture = m_texInput->getValue( data );
   if ( !texture )
   {
      // no input texture - no rendering
      return;
   }

   Renderer& renderer = host.getRenderer();
   Camera& activeCam = renderer.getActiveCamera();

   // render the whole scene to the back-buffer
   new ( renderer() ) RCActivateRenderTarget( NULL );

   // bind the shader
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader, renderer );
   {
      comm->setInt( "g_mode", (int)m_type );
      comm->setInt( "g_farZ", activeCam.getFarClippingPlane() );
      comm->setTexture( "g_Texture", *texture );
   }

   // determine the quad size ( take any of the defined render targets, since
   // MRT requires them all to have the same size anyway ).
   uint quadWidth = renderer.getViewportWidth();
   uint quadHeight = renderer.getViewportHeight();

   new ( renderer() ) RCFullscreenQuad( quadWidth, quadHeight );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_shader, renderer );

   // unbind render targets
   new ( renderer() ) RCDeactivateRenderTarget( 0 );
}

///////////////////////////////////////////////////////////////////////////////
