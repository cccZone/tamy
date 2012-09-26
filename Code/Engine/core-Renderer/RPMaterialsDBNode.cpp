#include "core-Renderer\RPMaterialsDBNode.h"
#include "core-Renderer\RenderingPipeline.h"
#include "core-Renderer\RenderingPipelineSockets.h"
#include "core-Renderer\RenderTarget.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\FullscreenQuad.h"
#include "core-Renderer\DepthBuffer.h"
#include "core-Renderer\PixelShader.h"
#include "core-Renderer\MaterialsDB.h"
#include "core-Renderer\MaterialInstance.h"
#include "core-Renderer\Defines.h"
#include "core-Renderer\RenderTargetDescriptor.h"
#include "core-Renderer\DepthBufferDescriptor.h"
#include "core-Renderer\RenderingPipelineTransaction.h"
#include "core-Renderer\Viewport.h"
#include "core\ResourcesManager.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPMaterialsDBNode );
   PARENT( RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPMaterialsDBNode::RPMaterialsDBNode()
   : m_materialsDB( MaterialsDB::getInstance() )
   , m_descriptorsOutput( new RPTextureOutput( "MatDescr" ) )
   , m_texturesAtlas( new RPTextureOutput( "TexAtlas" ) )
{
   defineInput( new RPVoidInput( "Input" ) );

   std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs;
   outputs.push_back( m_descriptorsOutput );
   outputs.push_back( m_texturesAtlas );
   defineOutputs( outputs );

   // load the shaders
   ResourcesManager& resMgr = ResourcesManager::getInstance();
   m_descriptorsShader = resMgr.create< PixelShader >( FilePath( MATERIALS_DB_DIR "materialDescriptors.tpsh" ), true );
   m_atlasShader = resMgr.create< PixelShader >( FilePath( MATERIALS_DB_DIR "texturesAtlas.tpsh" ), true );

}

///////////////////////////////////////////////////////////////////////////////

RPMaterialsDBNode::~RPMaterialsDBNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void RPMaterialsDBNode::onObjectLoaded()
{
   __super::onObjectLoaded();

   m_descriptorsOutput = static_cast< RPTextureOutput* >( findOutput( "MatDescr" ) );
   m_texturesAtlas = static_cast< RPTextureOutput* >( findOutput( "TexAtlas" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPMaterialsDBNode::onCreatePrerequisites( RenderingPipelineTransaction& transaction ) const
{
   // create necessary render targets and depth buffers

   // material descriptor render target
   {
      RenderTargetDescriptor* rtDesc = new RenderTargetDescriptor();
      rtDesc->setTargetID( "MatDescr" );
      rtDesc->setStaticSize( 512, 512 );
      rtDesc->setType( false );
      rtDesc->setUsage( TU_COLOR );

      transaction.addRenderTarget( rtDesc );
   }

   // material descriptor depth buffer
   {
      DepthBufferDescriptor* dbDesc = new DepthBufferDescriptor();
      dbDesc->setID( "MatDescr" );
      dbDesc->setSize( 512, 512 );

      transaction.addDepthBuffer( dbDesc );
   }

   // texture atlas render target
   {
      RenderTargetDescriptor* rtDesc = new RenderTargetDescriptor();
      rtDesc->setTargetID( "TexAtlas" );
      rtDesc->setStaticSize( 2048, 2048 );
      rtDesc->setType( false );
      rtDesc->setUsage( TU_COLOR );

      transaction.addRenderTarget( rtDesc );
   }

   // texture atlas depth buffer
   {
      DepthBufferDescriptor* dbDesc = new DepthBufferDescriptor();
      dbDesc->setID( "TexAtlas" );
      dbDesc->setSize( 2048, 2048 );

      transaction.addDepthBuffer( dbDesc );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPMaterialsDBNode::onCreateLayout( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   // register runtime members
   data.registerVar( m_dbTimestamp );
   data.registerVar( m_descriptorsRT );
   data.registerVar( m_descriptorsDB );
   data.registerVar( m_texturesAtlasRT );
   data.registerVar( m_texturesAtlasDB );

   data[ m_dbTimestamp ] = -1;

   // create render targets
   {
      RenderTarget* descriptorsRT = host.getRenderTarget( "MatDescr" );
      data[ m_descriptorsRT ] = descriptorsRT;
      data[ m_descriptorsDB ] = host.getDepthBuffer( "MatDescr" );

      RenderTarget* texturesAtlasRT = host.getRenderTarget( "TexAtlas" );
      data[ m_texturesAtlasRT ] = texturesAtlasRT;
      data[ m_texturesAtlasDB ] = host.getDepthBuffer( "TexAtlas" );

      m_descriptorsOutput->setValue( data, descriptorsRT );
      m_texturesAtlas->setValue( data, texturesAtlasRT );
   }
}

///////////////////////////////////////////////////////////////////////////////

void RPMaterialsDBNode::onUpdate( RenderingPipelineMechanism& host ) const
{
   if ( !m_descriptorsShader /* || !m_atlasShader*/ )
   {
      // no shader available
      return;
   }

   // first - check if we have the most up to date version of the atlases
   RuntimeDataBuffer& data = host.data();
   Renderer& renderer = host.getRenderer();

   int dbTimestamp = data[ m_dbTimestamp ];
   RenderTarget* descriptorsRT = data[ m_descriptorsRT ];
   if ( !descriptorsRT )
   {
      return;
   }

   int latestDBTimestamp = m_materialsDB.getTimestamp();
   bool targetsNeedRefreshing = descriptorsRT->needsRefreshing( renderer );

   if ( !targetsNeedRefreshing && dbTimestamp == latestDBTimestamp )
   {
      // nothing to do here
      return;
   }

   DepthBuffer* descriptorsDB = data[ m_descriptorsDB ];
   RenderTarget* texturesAtlasRT = data[ m_texturesAtlasRT ];
   DepthBuffer* texturesAtlasDB = data[ m_texturesAtlasDB ];
   if ( !descriptorsDB || !texturesAtlasRT || !texturesAtlasDB )
   {
      // there's nothing to render the atlases to
      return;
   }

   // collect material data
   Array< Vector > materialsData;
   Array< Vector > textureCoords;
   m_materialsDB.collectMaterialData( materialsData );
   m_materialsDB.generateTextureCoordinates( textureCoords );

   renderMaterialDescriptors( renderer, descriptorsRT, descriptorsDB, materialsData, textureCoords );
   //renderTextureAtlas( renderer, texturesAtlasRT, texturesAtlasDB, textureCoords );


   // we rebuilt the atlases, so now we need to memorize DB timestamp to make sure we don't
   // update them again if that's not gonna be necessary
   data[ m_dbTimestamp ] = latestDBTimestamp;
   descriptorsRT->markRefreshed( renderer );
}

///////////////////////////////////////////////////////////////////////////////

void RPMaterialsDBNode::renderMaterialDescriptors( Renderer& renderer, RenderTarget* rt, DepthBuffer* depthBuf, const Array< Vector >& materialsData, const Array< Vector >& textureCoords ) const
{
   // activate render target
   new ( renderer() ) RCActivateDepthBuffer( depthBuf );
   new ( renderer() ) RCActivateRenderTarget( rt );

   // we need to render that in multiple passes that
   const uint maxConstantsPerPass = 64;
   const uint dataCount = materialsData.size();
   const uint passesCount = dataCount / maxConstantsPerPass;
   
   Viewport viewport;
   viewport.m_offsetY = 0;
   viewport.m_width = rt->getWidth();
   viewport.m_height = rt->getHeight();
   viewport.m_minZ = -1;
   viewport.m_maxZ = 1;

   const uint quadHeight = 4;

   for ( uint startIdx = 0; startIdx < dataCount; startIdx += maxConstantsPerPass )
   {
      uint thisPassDataCount = min2( dataCount - startIdx, maxConstantsPerPass );
      uint quadWidth = thisPassDataCount / quadHeight;

      // set the viewport
      viewport.m_offsetX = startIdx;

      // setup the shader
      RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_descriptorsShader, renderer );
      {
         psComm->setVec4( "g_materialsData", ( (const Vector*)materialsData ) + startIdx, thisPassDataCount );
         psComm->setInt( "g_materialsCount", thisPassDataCount );
         psComm->setInt( "g_quadWidth", quadWidth );
         psComm->setInt( "g_dataStride", MaterialsDB::RENDER_DATA_BUFFER_STRIDE );
      }

      // render the quad
      new ( renderer() ) RCScreenQuad( startIdx, 0, quadWidth, 4 );
   }

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_descriptorsShader, renderer );
   new ( renderer() ) RCDeactivateRenderTarget();
   new ( renderer() ) RCDeactivateDepthBuffer( depthBuf );
}

///////////////////////////////////////////////////////////////////////////////

void RPMaterialsDBNode::renderTextureAtlas( Renderer& renderer, RenderTarget* rt, DepthBuffer* depthBuf,const Array< Vector >& textureCoords ) const
{
   // activate render target
   new ( renderer() ) RCActivateDepthBuffer( depthBuf );
   new ( renderer() ) RCActivateRenderTarget( rt );

   // render multiple passes, as many as there are textures to draw, each time setting a different viewport
   // and rendering a full quad with this texture just in that viewport

   // setup the shader
   RCBindPixelShader* psComm = new ( renderer() ) RCBindPixelShader( *m_atlasShader, renderer );
   {
      psComm->setVec4( "g_materialsData", (const Vector*)textureCoords, textureCoords.size() );
   }

   // render the quad
   uint width = rt->getWidth();
   uint height = rt->getHeight();
   new ( renderer() ) RCFullscreenQuad( width, height );

   // cleanup
   new ( renderer() ) RCUnbindPixelShader( *m_atlasShader, renderer );
   new ( renderer() ) RCDeactivateRenderTarget();
   new ( renderer() ) RCDeactivateDepthBuffer( depthBuf );
}

///////////////////////////////////////////////////////////////////////////////

