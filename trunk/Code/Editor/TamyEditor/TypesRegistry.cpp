
///////////////////////////////////////////////////////////////////////////////

#include "GraphBlock.h"

REGISTER_RTTI( GraphBlock );
REGISTER_RTTI( GraphBlockSocket );
REGISTER_RTTI( GraphBlockConnection );

///////////////////////////////////////////////////////////////////////////////

#include "RenderingPipelineSocketRepresentation.h"
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBStart.h"
#include "RPBAdapter.h"
#include "RPBDebugRender.h"
#include "RPBFloats2Vec4.h"
#include "RPBCamera.h"
#include "RPBTexture.h"
#include "RPBFloat.h"
#include "RPBVec4.h"
#include "RenderingPipelineLayout.h"

REGISTER_RTTI( RPBPostProcess );
REGISTER_RTTI( RPBSceneRender );
REGISTER_RTTI( RPBStart );
REGISTER_RTTI( RPBAdapter );
REGISTER_RTTI( RPBDebugRender );
REGISTER_RTTI( RPSFloat );
REGISTER_RTTI( RPSTexture );
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
REGISTER_RTTI( RenderingPipelineLayout );

///////////////////////////////////////////////////////////////////////////////

#include "MaterialLayout.h"
#include "MaterialSocketRepresentation.h"
#include "MBPixelShader.h"
#include "MBSpatialEntity.h"
#include "MBCamera.h"
#include "MBTexture.h"
#include "MBFloat.h"
#include "MBVec4.h"
#include "MBBool.h"
#include "MBInstanceTexture.h"
#include "MBSurfaceProperties.h"

REGISTER_RTTI( MaterialLayout );
REGISTER_RTTI( MSSFloat );
REGISTER_RTTI( MSSMatrix );
REGISTER_RTTI( MSSTexture );
REGISTER_RTTI( MSSVec4 );
REGISTER_RTTI( MSSBool );
REGISTER_RTTI( MBPixelShader );
REGISTER_RTTI( MBSpatialEntity );
REGISTER_RTTI( MBCamera );
REGISTER_RTTI( MBTexture );
REGISTER_RTTI( MBFloat );
REGISTER_RTTI( MBVec4 );
REGISTER_RTTI( MBBool );
REGISTER_RTTI( MBInstanceTexture );
REGISTER_RTTI( MBSurfaceProperties );

///////////////////////////////////////////////////////////////////////////////
