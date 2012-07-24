#include "core-Renderer/MRTUtil.h"
#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderTarget.h"
#include "core/StringUtils.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

void MRTUtil::defineOutputs( const std::string& renderTargetIds, RenderingPipelineNode* node )
{
   std::vector< std::string > renderTargets;
   StringUtils::tokenize( renderTargetIds, ";", renderTargets );

   // go through the existing sockets and create a list of sockets that correspond to removed render targets
   {
      std::vector< GBNodeOutput< RenderingPipelineNode >* > outputs = node->getOutputs();
      int outputsCount = outputs.size();
      for ( int outputIdx = outputsCount - 1; outputIdx >= 0; --outputIdx )
      {
         const std::string& outputName = outputs[outputIdx]->getName();
         bool stillExists = false;
         uint renderTargetsCount = renderTargets.size();
         for ( uint rtIdx = 0; rtIdx < renderTargetsCount; ++rtIdx )
         {
            if ( renderTargets[rtIdx] == outputName )
            {
               stillExists = true;
               renderTargets.erase( renderTargets.begin() + rtIdx );
               break;
            }
         }

         if ( stillExists )
         {
            outputs.erase( outputs.begin() + outputIdx );
         }
      }

      // remove the outputs that remain on the list
      outputsCount = outputs.size();
      for ( int outputIdx = outputsCount - 1; outputIdx >= 0; --outputIdx )
      {
         const std::string& outputName = outputs[outputIdx]->getName();
         node->removeOutput( outputName );
      }

      // now add new outputs
      uint renderTargetsCount = renderTargets.size();
      for ( uint rtIdx = 0; rtIdx < renderTargetsCount; ++rtIdx )
      {
         const std::string& rtName = renderTargets[rtIdx];
         node->defineOutput( new RPTextureOutput( rtName ) );
      }
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
