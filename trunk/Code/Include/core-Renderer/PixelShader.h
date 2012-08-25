/// @file   core-Renderer/PixelShader.h
/// @brief  a pixel shader resource
#ifndef _PIXEL_SHADER_H
#define _PIXEL_SHADER_H

#include "core/Resource.h"
#include "core-Renderer/ShaderRenderCommand.h"
#include "core/UniqueObject.h"
#include "core-Renderer/RenderResource.h"
#include "core-Renderer/RenderingParams.h"
#include "core/types.h"


///////////////////////////////////////////////////////////////////////////////

class Filesystem;
class RCBindPixelShader;
class InStream;
class OutStream;
class IDString;

///////////////////////////////////////////////////////////////////////////////

/**
 * Texture mapping params
 */
struct TextureStageParams
{
   TextureAddressingMode         m_addressU;
   TextureAddressingMode         m_addressV;
   TextureAddressingMode         m_addressW;
   TextureFilteringMode          m_minFilter;
   TextureFilteringMode          m_magFilter;
   TextureFilteringMode          m_mipFilter;

   TextureStageParams()
      : m_addressU( TAM_WRAP )
      , m_addressV( TAM_WRAP )
      , m_addressW( TAM_WRAP )
      , m_minFilter( TFM_LINEAR )
      , m_magFilter( TFM_LINEAR )
      , m_mipFilter( TFM_LINEAR )
   {}

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& stream, const TextureStageParams& params );
   friend InStream& operator>>( InStream& stream, TextureStageParams& params );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Drawing params
 */
struct PixelShaderParams
{
   CullingMode                   m_cullingMode;
   bool                          m_useZBuffer;
   bool                          m_writeToZBuffer;
   ComparisonFunc                m_depthTestFunc;

   bool                          m_useAlphaTest;
   ComparisonFunc                m_alphaTestFunc;
   ulong                         m_alphaTestRefVal;

   bool                          m_useBlending;
   BlendFunc                     m_blendSourceFunc;
   BlendFunc                     m_blendDestFunc;

   bool                          m_useSeparateAlphaBlend;
   BlendFunc                     m_alphaBlendSourceFunc;
   BlendFunc                     m_alphaBlendDestFunc;

   bool                          m_stencilEnable;
   StencilOp                     m_stencilFail;
   StencilOp                     m_stencilZFail;
   StencilOp                     m_stencilPass;
   ComparisonFunc                m_stencilFunc;
   ulong                         m_stencilRef;
   ulong                         m_stencilMask;
   ulong                         m_stencilWriteMask;


   PixelShaderParams()
      : m_cullingMode( CULL_CCW )
      , m_useZBuffer( true )
      , m_writeToZBuffer( true )
      , m_depthTestFunc( CMP_LESSEQUAL )
      , m_useAlphaTest( false )
      , m_alphaTestFunc( CMP_ALWAYS )
      , m_alphaTestRefVal( 1 )
      , m_useBlending( false )
      , m_blendSourceFunc( BLEND_ONE )
      , m_blendDestFunc( BLEND_ZERO )
      , m_useSeparateAlphaBlend( false )
      , m_alphaBlendSourceFunc( BLEND_ONE )
      , m_alphaBlendDestFunc( BLEND_ZERO )
      , m_stencilEnable( false )
      , m_stencilFail( STENCILOP_KEEP )
      , m_stencilZFail( STENCILOP_KEEP )
      , m_stencilPass( STENCILOP_KEEP )
      , m_stencilFunc( CMP_ALWAYS )
      , m_stencilRef( 0 )
      , m_stencilMask( 0xFFFFFFFF )
      , m_stencilWriteMask( 0xFFFFFFFF )
   {}

   // -------------------------------------------------------------------------
   // Serialization support
   // -------------------------------------------------------------------------
   friend OutStream& operator<<( OutStream& stream, const PixelShaderParams& params );
   friend InStream& operator>>( InStream& stream, PixelShaderParams& params );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a pixel shader program resource.
 */
class PixelShader : public Resource, public UniqueObject< PixelShader >, public RenderResource
{
   DECLARE_RESOURCE()

private:
   // static data
   std::string                               m_script;
   std::string                               m_entryFunctionName;
   PixelShaderParams                         m_params;
   std::vector< TextureStageParams >         m_textureStages;
   uint                                      m_requiredVertexShaderTechniqueId;

   // runtime data
   std::vector< std::string >                m_textureStageName;

public:
   /**
    * Constructor.
    *
    * @param resourceName             shader resource name
    */
   PixelShader( const FilePath& resourceName = FilePath() );
   ~PixelShader();

   /**
    * Returns the HLSL script of this shader.
    *
    * @return  HLSL shader script
    */
   inline const std::string& getScript() const { return m_script; }

   /**
    * Sets a new script.
    *
    * @param script
    */
   void setScript( const std::string& script );

   /**
    * Returns the number of texture stages count.
    */
   inline unsigned int getTextureStagesCount() const { return m_textureStages.size(); }

   /**
    * Returns the params used for texture mapping.
    *
    * @param stageIdx
    */
   inline const TextureStageParams& getTextureStage( unsigned int stageIdx ) const { return m_textureStages[stageIdx]; }

   /**
    * Returns the params used for texture mapping (non-const version) so they can be changed.
    *
    * @param stageIdx
    */
   inline TextureStageParams& changeTextureStage( unsigned int stageIdx ) { setDirty(); return m_textureStages[stageIdx]; }

   /**
    * Returns the name of a texture stage.
    *
    * @param stageIdx
    */
   inline const std::string getTextureStageName( unsigned int stageIdx ) const { return m_textureStageName[stageIdx]; }

   /**
    * Returns the drawing params.
    */
   inline const PixelShaderParams& getParams() const { return m_params; }

   /**
    * Returns the drawing params (non-const version) so they can be changed.
    */
   inline PixelShaderParams& changeParams() { setDirty(); return m_params; }

   /**
    * Returns the name of the shader entry function
    */
   inline const std::string& getEntryFunctionName() const { return m_entryFunctionName; }

   /**
    * Returns the id of a vertex shader technique this pixel shader requires.
    */
   inline uint getRequiredVertexShaderTechnique() const { return m_requiredVertexShaderTechniqueId; }

   /**
    * Creates a texture setting shader parameter for the effect shader.
    *
    * @param allocator
    * @param paramName
    * @param val
    */
   static ShaderParam< PixelShader >* createTextureSetter( MemoryPoolAllocator& allocator, const IDString& paramName, ShaderTexture& val );

   // -------------------------------------------------------------------------
   // Resource implementation
   // -------------------------------------------------------------------------
   void onResourceLoaded( ResourcesManager& mgr );

private:
   void parseTextureStages();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Binds a pixel shader to the render device.
 */
class RCBindPixelShader : public ShaderRenderCommand< PixelShader >
{
private:
   PixelShader&               m_shader;
   
public:
   /**
    * Constructor.
    */
   RCBindPixelShader( PixelShader& shader, Renderer& renderer );

   // -------------------------------------------------------------------------
   // Shader program setters
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Binds a pixel shader to the render device.
 */
class RCUnbindPixelShader : public RenderCommand
{
private:
   PixelShader&         m_shader;

public:
   /**
    * Constructor.
    */
   RCUnbindPixelShader( PixelShader& shader, Renderer& renderer );

   // -------------------------------------------------------------------------
   // Shader program setters
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

#endif // _PIXEL_SHADER_H
