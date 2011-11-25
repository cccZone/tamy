#include "core-Renderer.h"


///////////////////////////////////////////////////////////////////////////////

REGISTER_RTTI( EffectShader );
REGISTER_RTTI( SingleTextureEffect );

///////////////////////////////////////////////////////////////////////////////

REGISTER_RTTI( Geometry );
REGISTER_RTTI( GeometryResource );
REGISTER_RTTI( StaticGeometry );
REGISTER_RTTI( SkinnedGeometry );
REGISTER_RTTI( LineSegments );
REGISTER_RTTI( Skeleton );
REGISTER_RTTI( TriangleMesh );

///////////////////////////////////////////////////////////////////////////////

REGISTER_RTTI( ShaderTexture );
REGISTER_RTTI( SingleTextureMaterial );
REGISTER_RTTI( MaterialEntity );
REGISTER_RTTI( VertexShader );
REGISTER_RTTI( PixelShader );
REGISTER_RTTI( FragmentShader );
REGISTER_RTTI( Texture );
REGISTER_RTTI( RenderTarget );
REGISTER_RTTI( SurfaceProperties );
REGISTER_ENUM_RTTI( VertexDescId );

///////////////////////////////////////////////////////////////////////////////

REGISTER_RTTI( Material );
REGISTER_RTTI( MaterialNode );
REGISTER_RTTI( MNCamera );
REGISTER_RTTI( MNFloat );
REGISTER_RTTI( MNPixelShader );
REGISTER_RTTI( MNSpatialEntity );
REGISTER_RTTI( MNTexture );
REGISTER_RTTI( MNVec4 );
REGISTER_RTTI( MNBool );
REGISTER_RTTI( MNInstanceTexture );
REGISTER_RTTI( MNSurfaceProperties );
REGISTER_RTTI( MSFloatInput );
REGISTER_RTTI( MSFloatOutput );
REGISTER_RTTI( MSMatrixInput );
REGISTER_RTTI( MSMatrixOutput );
REGISTER_RTTI( MSTextureInput );
REGISTER_RTTI( MSTextureOutput );
REGISTER_RTTI( MSVec4Input );
REGISTER_RTTI( MSVec4Output );
REGISTER_RTTI( MSBoolInput );
REGISTER_RTTI( MSBoolOutput );
REGISTER_ENUM_RTTI( MaterialTextures );

///////////////////////////////////////////////////////////////////////////////

REGISTER_RTTI( RenderingPipeline );
REGISTER_RTTI( RenderingPipelineNode );
REGISTER_RTTI( RPSceneRenderNode );
REGISTER_RTTI( RPPostProcessNode );
REGISTER_RTTI( RPStartNode );
REGISTER_RTTI( RPDebugRendererNode );
REGISTER_RTTI( RPAdapterNode );
REGISTER_RTTI( RPCameraNode );
REGISTER_RTTI( RPFloats2Vec4Node );
REGISTER_RTTI( RPTextureNode );
REGISTER_RTTI( RPFloatNode );
REGISTER_RTTI( RPVec4Node );
REGISTER_RTTI( RPFloatInput );
REGISTER_RTTI( RPFloatOutput );
REGISTER_RTTI( RPTextureInput );
REGISTER_RTTI( RPTextureOutput );
REGISTER_RTTI( RPVoidInput );
REGISTER_RTTI( RPVoidOutput );
REGISTER_RTTI( RPBoolInput );
REGISTER_RTTI( RPBoolOutput );
REGISTER_RTTI( RPIntInput );
REGISTER_RTTI( RPIntOutput );
REGISTER_RTTI( RPStringInput );
REGISTER_RTTI( RPStringOutput );
REGISTER_RTTI( RPVec4Input );
REGISTER_RTTI( RPVec4Output );
REGISTER_RTTI( RPMatrixInput );
REGISTER_RTTI( RPMatrixOutput );
REGISTER_RTTI( RenderTargetDescriptor );
REGISTER_RTTI( RPSceneBuilder );
REGISTER_RTTI( RPSBTextured );
REGISTER_RTTI( RPSBComputed );
REGISTER_ENUM_RTTI( TextureUsage );
REGISTER_ENUM_RTTI( RPMSceneId );

///////////////////////////////////////////////////////////////////////////////
