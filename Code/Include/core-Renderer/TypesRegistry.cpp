#include "core/TypeRegistrationMacros.h"
#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

REGISTER_TYPE( EffectShader );
REGISTER_TYPE( SingleTextureEffect );

///////////////////////////////////////////////////////////////////////////////

REGISTER_ABSTRACT_TYPE( Geometry );
REGISTER_ABSTRACT_TYPE( GeometryResource );
REGISTER_TYPE( StaticGeometry );
REGISTER_TYPE( SkinnedGeometry );
REGISTER_TYPE( GeometryEntity );
REGISTER_EXTERNAL_TYPE( LitVertex );
REGISTER_EXTERNAL_TYPE( VertexWeight );
REGISTER_EXTERNAL_TYPE( Face );
REGISTER_EXTERNAL_TYPE( LineSegment );
REGISTER_TYPE( LineSegments );
REGISTER_TYPE( Skeleton );
REGISTER_TYPE( TriangleMesh );

///////////////////////////////////////////////////////////////////////////////

REGISTER_ABSTRACT_TYPE( ShaderTexture );
REGISTER_TYPE( MaterialEntity );
REGISTER_TYPE( VertexShader );
REGISTER_TYPE( PixelShader );
REGISTER_EXTERNAL_TYPE( PixelShaderParams );
REGISTER_EXTERNAL_TYPE( TextureStageParams );
REGISTER_TYPE( FragmentShader );
REGISTER_TYPE( Texture );
REGISTER_TYPE( RenderTarget );
REGISTER_EXTERNAL_TYPE( SurfaceProperties );
REGISTER_ENUM_TYPE( VertexDescId );

///////////////////////////////////////////////////////////////////////////////

REGISTER_TYPE( Material );
REGISTER_ABSTRACT_TYPE( MaterialNode );
DEFINE_TYPE_ID( GBNodeOutput<MaterialNode> );
MAP_TYPE( GBNodeInput<class MaterialNode>, ReflectionObject );
REGISTER_TYPE( MNCamera );
REGISTER_TYPE( MNFloat );
REGISTER_TYPE( MNPixelShader );
REGISTER_TYPE( MNSpatialEntity );
REGISTER_TYPE( MNTexture );
REGISTER_TYPE( MNVec4 );
REGISTER_TYPE( MNBool );
REGISTER_TYPE( MNInstanceTexture );
REGISTER_TYPE( MNSurfaceProperties );
REGISTER_TYPE( MNTimer );
REGISTER_TYPE( MSFloatInput );
REGISTER_TYPE( MSFloatOutput );
REGISTER_TYPE( MSMatrixInput );
REGISTER_TYPE( MSMatrixOutput );
REGISTER_TYPE( MSTextureInput );
REGISTER_TYPE( MSTextureOutput );
REGISTER_TYPE( MSVec4Input );
REGISTER_TYPE( MSVec4Output );
REGISTER_TYPE( MSBoolInput );
REGISTER_TYPE( MSBoolOutput );
REGISTER_ENUM_TYPE( MaterialTextures );

///////////////////////////////////////////////////////////////////////////////

REGISTER_TYPE( GeometryShader );
REGISTER_ABSTRACT_TYPE( GeometryShaderNode );
DEFINE_TYPE_ID( GBNodeOutput<GeometryShaderNode> );
MAP_TYPE( GBNodeInput<GeometryShaderNode>, ReflectionObject );
REGISTER_TYPE( GNCamera );
REGISTER_TYPE( GNFloat );
REGISTER_TYPE( GNVertexShader );
REGISTER_TYPE( GNSpatialEntity );
REGISTER_TYPE( GNTexture );
REGISTER_TYPE( GNVec4 );
REGISTER_TYPE( GNBool );
REGISTER_TYPE( GNTimer );
REGISTER_TYPE( GSFloatInput );
REGISTER_TYPE( GSFloatOutput );
REGISTER_TYPE( GSMatrixInput );
REGISTER_TYPE( GSMatrixOutput );
REGISTER_TYPE( GSTextureInput );
REGISTER_TYPE( GSTextureOutput );
REGISTER_TYPE( GSVec4Input );
REGISTER_TYPE( GSVec4Output );
REGISTER_TYPE( GSBoolInput );
REGISTER_TYPE( GSBoolOutput );

///////////////////////////////////////////////////////////////////////////////

REGISTER_TYPE( RenderingPipeline );
REGISTER_ABSTRACT_TYPE( RenderingPipelineNode );
DEFINE_TYPE_ID( GBNodeOutput<RenderingPipelineNode> );
MAP_TYPE( GBNodeInput<RenderingPipelineNode>, ReflectionObject );
REGISTER_TYPE( RPSceneRenderNode );
REGISTER_TYPE( RPPostProcessNode );
REGISTER_TYPE( RPStartNode );
REGISTER_TYPE( RPAdapterNode );
REGISTER_TYPE( RPCameraNode );
REGISTER_TYPE( RPFloats2Vec4Node );
REGISTER_TYPE( RPTextureNode );
REGISTER_TYPE( RPFloatNode );
REGISTER_TYPE( RPVec4Node );
REGISTER_TYPE( RPFloatInput );
REGISTER_TYPE( RPFloatOutput );
REGISTER_TYPE( RPTextureInput );
REGISTER_TYPE( RPTextureOutput );
REGISTER_TYPE( RPVoidInput );
REGISTER_TYPE( RPVoidOutput );
REGISTER_TYPE( RPBoolInput );
REGISTER_TYPE( RPBoolOutput );
REGISTER_TYPE( RPIntInput );
REGISTER_TYPE( RPIntOutput );
REGISTER_TYPE( RPStringInput );
REGISTER_TYPE( RPStringOutput );
REGISTER_TYPE( RPVec4Input );
REGISTER_TYPE( RPVec4Output );
REGISTER_TYPE( RPMatrixInput );
REGISTER_TYPE( RPMatrixOutput );
REGISTER_TYPE( RenderTargetDescriptor );
REGISTER_TYPE( DepthBufferDescriptor );
REGISTER_ENUM_TYPE( TextureUsage );

///////////////////////////////////////////////////////////////////////////////

REGISTER_ABSTRACT_TYPE( Light );
REGISTER_TYPE( DirectionalLight );
REGISTER_TYPE( PointLight );
REGISTER_TYPE( RPDeferredLightingNode );
REGISTER_TYPE( RPSoftShadowsNode );

///////////////////////////////////////////////////////////////////////////////
