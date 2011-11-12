
///////////////////////////////////////////////////////////////////////////////

#include "GraphBlock.h"
#include "RenderingPipelineBlock.h"
#include "RenderingPipelineSocketRepresentation.h"
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBDepthNormals.h"
#include "RPBStart.h"
#include "RPBAdapter.h"
#include "RPBDebugRender.h"
#include "RPSFloat.h"
#include "RPSTexture.h"
#include "RPSVoid.h"
#include "RenderingPipelineLayout.h"

REGISTER_RTTI( GraphBlock );
REGISTER_RTTI( RenderingPipelineBlock );
REGISTER_RTTI( GraphBlockSocket );
REGISTER_RTTI( RenderingPipelineSocketRepresentation );
REGISTER_RTTI( RPBPostProcess );
REGISTER_RTTI( RPBSceneRender );
REGISTER_RTTI( RPBDepthNormals );
REGISTER_RTTI( RPBStart );
REGISTER_RTTI( RPBAdapter );
REGISTER_RTTI( RPBDebugRender );
REGISTER_RTTI( RPSFloat );
REGISTER_RTTI( RPSTextureInput );
REGISTER_RTTI( RPSTextureOutput );
REGISTER_RTTI( RPSVoid );
REGISTER_RTTI( GraphBlockConnection );
REGISTER_RTTI( RenderingPipelineLayout );

///////////////////////////////////////////////////////////////////////////////
