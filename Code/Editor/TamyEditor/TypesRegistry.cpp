#include "core/TypeRegistrationMacros.h"


///////////////////////////////////////////////////////////////////////////////

#include "Project.h"
REGISTER_TYPE( Project );

///////////////////////////////////////////////////////////////////////////////

#include "GraphBlock.h"

REGISTER_TYPE( GraphBlock );
REGISTER_TYPE( GraphBlockSocket );
REGISTER_TYPE( GraphBlockConnection );

///////////////////////////////////////////////////////////////////////////////

#include "RenderingPipelineSocketRepresentation.h"
#include "RPBPostProcess.h"
#include "RPBSceneRender.h"
#include "RPBStart.h"
#include "RPBAdapter.h"
#include "RPBFloats2Vec4.h"
#include "RPBCamera.h"
#include "RPBTexture.h"
#include "RPBFloat.h"
#include "RPBVec4.h"
#include "RPBDeferredLighting.h"
#include "RenderingPipelineLayout.h"

REGISTER_TYPE( RPBPostProcess );
REGISTER_TYPE( RPBSceneRender );
REGISTER_TYPE( RPBStart );
REGISTER_TYPE( RPBAdapter );
REGISTER_TYPE( RPSFloat );
REGISTER_TYPE( RPSTexture );
REGISTER_TYPE( RPSVoid );
REGISTER_TYPE( RPSVec4 );
REGISTER_TYPE( RPSBool );
REGISTER_TYPE( RPSInt );
REGISTER_TYPE( RPSString );
REGISTER_TYPE( RPSMatrix );
REGISTER_TYPE( RPBFloats2Vec4 );
REGISTER_TYPE( RPBCamera );
REGISTER_TYPE( RPBTexture );
REGISTER_TYPE( RPBFloat );
REGISTER_TYPE( RPBVec4 );
REGISTER_TYPE( RPBDeferredLighting );
REGISTER_TYPE( RenderingPipelineLayout );

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
#include "MBTimer.h"

REGISTER_TYPE( MaterialLayout );
REGISTER_TYPE( MSSFloat );
REGISTER_TYPE( MSSMatrix );
REGISTER_TYPE( MSSTexture );
REGISTER_TYPE( MSSVec4 );
REGISTER_TYPE( MSSBool );
REGISTER_TYPE( MBPixelShader );
REGISTER_TYPE( MBSpatialEntity );
REGISTER_TYPE( MBCamera );
REGISTER_TYPE( MBTexture );
REGISTER_TYPE( MBFloat );
REGISTER_TYPE( MBVec4 );
REGISTER_TYPE( MBBool );
REGISTER_TYPE( MBInstanceTexture );
REGISTER_TYPE( MBSurfaceProperties );
REGISTER_TYPE( MBTimer );

///////////////////////////////////////////////////////////////////////////////

#include "GeometryShaderLayout.h"
#include "GeometryShaderSocketRepresentation.h"
#include "GBVertexShader.h"
#include "GBSpatialEntity.h"
#include "GBCamera.h"
#include "GBTexture.h"
#include "GBFloat.h"
#include "GBVec4.h"
#include "GBBool.h"
#include "GBTimer.h"

REGISTER_TYPE( GeometryShaderLayout );
REGISTER_TYPE( GSSFloat );
REGISTER_TYPE( GSSMatrix );
REGISTER_TYPE( GSSTexture );
REGISTER_TYPE( GSSVec4 );
REGISTER_TYPE( GSSBool );
REGISTER_TYPE( GBVertexShader );
REGISTER_TYPE( GBSpatialEntity );
REGISTER_TYPE( GBCamera );
REGISTER_TYPE( GBTexture );
REGISTER_TYPE( GBFloat );
REGISTER_TYPE( GBVec4 );
REGISTER_TYPE( GBBool );
REGISTER_TYPE( GBTimer );

///////////////////////////////////////////////////////////////////////////////

#include "NavigationState.h"
#include "NodeManipulationState.h"

REGISTER_TYPE( NavigationState );
REGISTER_TYPE( NodeManipulationState );

///////////////////////////////////////////////////////////////////////////////

REGISTER_EXTERNAL_TYPE( QPointF );

///////////////////////////////////////////////////////////////////////////////

#include "DebugGeometry.h"
#include "DRDirectionalLight.h"
#include "DRPointLight.h"
#include "GizmoMaterial.h"
#include "GizmoAxis.h"
#include "Gizmo.h"
#include "DebugGrid.h"
#include "SingleColorDebugMat.h"
REGISTER_ABSTRACT_TYPE( GizmoMaterial );
REGISTER_ABSTRACT_TYPE( GizmoAxis );
REGISTER_ABSTRACT_TYPE( Gizmo );
REGISTER_ABSTRACT_TYPE( DebugGrid );
REGISTER_ABSTRACT_TYPE( SingleColorDebugMat );
REGISTER_ABSTRACT_TYPE( DebugGeometry );
REGISTER_ABSTRACT_TYPE( DRDirectionalLight );
REGISTER_ABSTRACT_TYPE( DRPointLight );

///////////////////////////////////////////////////////////////////////////////
