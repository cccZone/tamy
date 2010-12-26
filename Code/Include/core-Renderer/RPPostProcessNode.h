/// @file   core-Renderer/RPPostProcessNode.h
/// @brief  a rendering pipeline node responsible for applying a post process effect.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPPostProcessNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPPostProcessNode )

private:
   int         m_testVal;
};

///////////////////////////////////////////////////////////////////////////////
