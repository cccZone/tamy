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

REGISTER_RTTI( SingleTextureMaterial );
REGISTER_RTTI( VertexShader );
REGISTER_RTTI( PixelShader );
REGISTER_RTTI( Texture );
REGISTER_RTTI( RenderTarget );
REGISTER_RTTI( Material );

///////////////////////////////////////////////////////////////////////////////

REGISTER_RTTI( RenderingPipeline );
REGISTER_RTTI( RenderingPipelineNode );
REGISTER_RTTI( RPSceneRenderNode );
REGISTER_RTTI( RPPostProcessNode );
REGISTER_RTTI( RPStartNode );
REGISTER_RTTI( RPDebugRendererNode );
REGISTER_RTTI( RPBlurNode );
REGISTER_RTTI( RPBrightnessNode );
REGISTER_RTTI( RPDownsampleNode );
REGISTER_RTTI( RPHDRNode );
REGISTER_RTTI( RPLuminanceNode );
REGISTER_RTTI( RPAdapterNode );
REGISTER_RTTI( RPNodeSocket );
REGISTER_RTTI( RPNodeInput );
REGISTER_RTTI( RPNodeOutput );
REGISTER_RTTI( RPFloatInput );
REGISTER_RTTI( RPFloatOutput );
REGISTER_RTTI( RPTextureInput );
REGISTER_RTTI( RPTextureOutput );
REGISTER_RTTI( RPVoidInput );
REGISTER_RTTI( RPVoidOutput );
REGISTER_RTTI( RenderTargetDescriptor );
REGISTER_ENUM_RTTI( BlurOptions );
REGISTER_ENUM_RTTI( RPMSceneId );

///////////////////////////////////////////////////////////////////////////////
