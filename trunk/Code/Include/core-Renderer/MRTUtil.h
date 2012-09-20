/// @file   core-Renderer/MRTUtil.h
/// @brief  a utility class for adding support for multiple render targets ( MRT )
#pragma once

#include <string>
#include "core/Array.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineMechanism;
class RenderingPipelineNode;
class RenderTarget;

///////////////////////////////////////////////////////////////////////////////

class MRTUtil
{
public:

   /**
    * Defines new outputs on a rendering pipeline block based on the ;-separated list
    * of render target names.
    *
    * @param renderTargetIds        ;-separated list of render target names
    * @param node                   modified rendering pipeline node
    */
   static void defineOutputs( const std::string& renderTargetIds, RenderingPipelineNode* node );

   /**
    * Rebuilds a list of render targets for the specific pipeline instance.
    *
    * @param host                   host pipeline instance that contains the definitions of render targets
    * @param node                   modified rendering pipeline node
    * @param outRenderTargets       list of render targets
    */
   static void refreshRenderTargets( RenderingPipelineMechanism& host, const RenderingPipelineNode* node, Array< RenderTarget* >& outRenderTargets );

   /**
    * Rebuilds a list of render targets for the specific pipeline instance and for the specific outputs only
    *
    * @param host                   host pipeline instance that contains the definitions of render targets
    * @param node                   modified rendering pipeline node
    * @param renderTargetIds        ;-separated list of render target names
    * @param outRenderTargets       list of render targets
    */
   static void refreshRenderTargets( RenderingPipelineMechanism& host, const RenderingPipelineNode* node, const std::string& renderTargetIds, Array< RenderTarget* >& outRenderTargets );

};

///////////////////////////////////////////////////////////////////////////////
