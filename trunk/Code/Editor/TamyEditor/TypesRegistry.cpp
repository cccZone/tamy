
///////////////////////////////////////////////////////////////////////////////

#include "GraphBlock.h"
#include "RenderingPipelineBlock.h"
#include "RenderingPipelineSocketRepresentation.h"
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBStart.h"
#include "RPBAdapter.h"
#include "RPBDebugRender.h"
#include "RPSFloat.h"
#include "RPSTexture.h"
#include "RPSVoid.h"
#include "RPSVec4.h"
#include "RPSBool.h"
#include "RPSInt.h"
#include "RPSString.h"
#include "RPSMatrix.h"
#include "RPBFloats2Vec4.h"
#include "RPBCamera.h"
#include "RPBTexture.h"
#include "RPBFloat.h"
#include "RPBVec4.h"
#include "RenderingPipelineLayout.h"

REGISTER_RTTI( GraphBlock );
REGISTER_RTTI( RenderingPipelineBlock );
REGISTER_RTTI( GraphBlockSocket );
REGISTER_RTTI( RenderingPipelineSocketRepresentation );
REGISTER_RTTI( RPBPostProcess );
REGISTER_RTTI( RPBSceneRender );
REGISTER_RTTI( RPBStart );
REGISTER_RTTI( RPBAdapter );
REGISTER_RTTI( RPBDebugRender );
REGISTER_RTTI( RPSFloat );
REGISTER_RTTI( RPSTextureInput );
REGISTER_RTTI( RPSRenderTargetOutput );
REGISTER_RTTI( RPSImageOutput );
REGISTER_RTTI( RPSVoid );
REGISTER_RTTI( RPSVec4 );
REGISTER_RTTI( RPSBool );
REGISTER_RTTI( RPSInt );
REGISTER_RTTI( RPSString );
REGISTER_RTTI( RPSMatrix );
REGISTER_RTTI( RPBFloats2Vec4 );
REGISTER_RTTI( RPBCamera );
REGISTER_RTTI( RPBTexture );
REGISTER_RTTI( RPBFloat );
REGISTER_RTTI( RPBVec4 );
REGISTER_RTTI( GraphBlockConnection );
REGISTER_RTTI( RenderingPipelineLayout );

///////////////////////////////////////////////////////////////////////////////
