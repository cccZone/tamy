#include "core-Renderer/MRTUtil.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderTarget.h"
#include "core/StringUtils.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

void MRTUtil::createOutputs( const std::string& renderTargetIds, std::vector< GBNodeOutput< RenderingPipelineNode >* >& outNodes )
{
   std::vector< std::string > renderTargets;
   StringUtils::tokenize( renderTargetIds, ";", renderTargets );

   // now add new outputs
   uint renderTargetsCount = renderTargets.size();
   for ( uint rtIdx = 0; rtIdx < renderTargetsCount; ++rtIdx )
   {
      const std::string& rtName = renderTargets[rtIdx];
      outNodes.push_back( new RPTextureOutput( rtName ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MRTUtil::refreshRenderTargets( RenderingPipelineMechanism& host, const RenderingPipelineNode* node, Array< RenderTarget* >& outRenderTargets )
{
   RuntimeDataBuffer& data = host.data();
   // remove old render targets
   outRenderTargets.clear();

   // acquire the new ones
   std::vector< GBNodeOutput< RenderingPipelineNode >* > renderTargetOutputs = node->getOutputs();
   uint count = renderTargetOutputs.size();
   outRenderTargets.resize( count );
   for ( uint i = 0; i < count; ++i )
   {
      RPTextureOutput* rtOutput = static_cast< RPTextureOutput* >( renderTargetOutputs[i] );
      const std::string& renderTargetName = rtOutput->getName();

      RenderTarget* target = host.getRenderTarget( renderTargetName );

      // we'll either find a target with the name we defined, or we're going to be rendering directly to back buffer.
      outRenderTargets[i] = target;
      rtOutput->setValue( data, target );
   }
}

///////////////////////////////////////////////////////////////////////////////

void MRTUtil::refreshRenderTargets( RenderingPipelineMechanism& host, const RenderingPipelineNode* node, const std::string& renderTargetIds, Array< RenderTarget* >& outRenderTargets )
{
   RuntimeDataBuffer& data = host.data();
   // remove old render targets
   outRenderTargets.clear();

   std::vector< std::string > renderTargets;
   StringUtils::tokenize( renderTargetIds, ";", renderTargets );

   // acquire the new ones
   uint count = renderTargets.size();
   outRenderTargets.resize( count );
   for ( uint i = 0; i < count; ++i )
   {
      GBNodeOutput< RenderingPipelineNode >* output = node->findOutput( renderTargets[i] );
      RPTextureOutput* rtOutput = static_cast< RPTextureOutput* >( output );
      const std::string& renderTargetName = rtOutput->getName();

      RenderTarget* target = host.getRenderTarget( renderTargetName );

      // we'll either find a target with the name we defined, or we're going to be rendering directly to back buffer.
      outRenderTargets[i] = target;
      rtOutput->setValue( data, target );
   }
}

///////////////////////////////////////////////////////////////////////////////
