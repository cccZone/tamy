#pragma once

/// @file   core-Renderer\RenderTarget.h
/// @brief  a special kind of texture which a renderer uses as an output

#include "core\Observer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core-Renderer\RenderState.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\ShaderParam.h"
#include "core-Renderer\RenderResource.h"
#include "core\UniqueObject.h"
#include "core\Color.h"
#include <string>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class Renderer;
enum RendererOps;
class RenderTargetSizePolicy;
enum TextureFormat;

///////////////////////////////////////////////////////////////////////////////

/**
 * A special kind of texture which a renderer uses as an output.
 */
class RenderTarget : public TRenderState< RenderTarget >, public ShaderTexture, public UniqueObject< RenderTarget >, public RenderResource
{
   DECLARE_RTTI_CLASS

private:
   RenderTargetSizePolicy*       m_sizePolicy;
   TextureUsage                  m_usage;
   bool                          m_isReadable;
   Color                         m_bgColor;

   unsigned int                  m_width;
   unsigned int                  m_height;

public:
   /**
    * Constructor.
    *
    * @param sizePolicy    size policy
    * @param usage         render target usage
    * @param isReadable    tells if we can perform reading operations on the texture
    * @param bgColor       color the background of the target should be filled with
    */
   RenderTarget( RenderTargetSizePolicy* sizePolicy = NULL, TextureUsage usage = TU_COLOR, bool isReadable = false, const Color& bgColor = Color( 0.f, 0.f, 0.f, 0.f ) );
   ~RenderTarget();

   /**
    * Sets the new size of the render target.
    */
   void resize( unsigned int width, unsigned int height );

   /**
    * Returns the render target usage.
    */
   inline TextureUsage getUsage() const { return m_usage; }

   /**
    * Tells if the texture supports reading operation.
    */
   inline bool isReadable() const { return m_isReadable; }

   /**
    * Returns the color the target's background should be filled with
    * when it's being cleaned.
    */
   inline const Color& getBackgroundColor() const { return m_bgColor; }

   /**
    * Returns the color of the specified pixel.
    *
    * @throw std::logic_error    if the texture doesn't support reading operation
    *
    * @param pos     pixel position
    */
   Color getPixel( const D3DXVECTOR2& pos ) const;

   // -------------------------------------------------------------------------
   // RenderState implementation
   // -------------------------------------------------------------------------
   void onPreRender( Renderer& renderer );
   void onPostRender( Renderer& renderer );
   bool onEquals( const RenderTarget& rhs ) const;
   bool onLess( const RenderTarget& rhs ) const;

   // -------------------------------------------------------------------------
   // ShaderTexture implementation
   // -------------------------------------------------------------------------
   inline unsigned int getWidth() const { return m_width; }
   inline unsigned int getHeight() const { return m_height; }
   inline ShaderParam< EffectShader >* createEffectShaderTextureSetter( const std::string& paramName ) { return new ShaderParamRenderTarget< EffectShader >( paramName, *this ); }
   inline ShaderParam< PixelShader >* createPixelShaderTextureSetter( const std::string& paramName ) { return new ShaderParamRenderTarget< PixelShader >( paramName, *this ); }
   inline ShaderParam< VertexShader >* createVertexShaderTextureSetter( const std::string& paramName ){ return new ShaderParamRenderTarget< VertexShader >( paramName, *this ); }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Size policy scales a render target.
 */
class RenderTargetSizePolicy
{
public:
   virtual ~RenderTargetSizePolicy() {}

   /**
    * Initializes the policy.
    *
    * @param target        host render target
    */
   virtual void initialize( RenderTarget& target ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Dynamically adjusts render target's size to the size of an active viewport
 */
class RTSPDynamic : public RenderTargetSizePolicy, public Observer< Renderer, RendererOps >
{
private:
   Renderer&         m_renderer;
   float             m_widthScale;
   float             m_heightScale;

   RenderTarget*     m_hostTarget;

public:
   RTSPDynamic( Renderer& renderer, float widthScale = 1.f, float heightScale = 1.f );
   ~RTSPDynamic();

   // -------------------------------------------------------------------------
   // RenderTargetSizePolicy implementation
   // -------------------------------------------------------------------------
   void initialize( RenderTarget& target );

   // -------------------------------------------------------------------------
   // Renderer observer implementation
   // -------------------------------------------------------------------------
   void update(Renderer& renderer);
   void update(Renderer& renderer, const RendererOps& operation);
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Defines a single size for the render target.
 */
class RTSPStatic : public RenderTargetSizePolicy
{
private:
   unsigned int      m_width;
   unsigned int      m_height;

public:
   RTSPStatic( unsigned int width, unsigned int height );

   // -------------------------------------------------------------------------
   // RenderTargetSizePolicy implementation
   // -------------------------------------------------------------------------
   void initialize( RenderTarget& target );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A dynamic render target policy that matches the size to the size 
 * of another texture.
 */
class RTSPTexture : public RenderTargetSizePolicy, public Observer< ShaderTexture, ShaderTextureOps >
{
private:
   Renderer&         m_renderer;
   ShaderTexture&    m_texture;
   RenderTarget*     m_hostTarget;

public:
   /**
    * Constructor.
    *
    * @param renderer   renderer on which the render target should be implemented
    * @param texture    tracked texture
    */
   RTSPTexture( Renderer& renderer, ShaderTexture& texture );
   ~RTSPTexture();

   // -------------------------------------------------------------------------
   // RenderTargetSizePolicy implementation
   // -------------------------------------------------------------------------
   void initialize( RenderTarget& target );

   // -------------------------------------------------------------------------
   // Renderer observer implementation
   // -------------------------------------------------------------------------
   void update( ShaderTexture& texture );
   void update( ShaderTexture& texture, const ShaderTextureOps& operation );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command that ensures a render target existence.
 */
class RCCreateRenderTarget : public RenderCommand
{
private:
   RenderTarget&    m_rt;

public:
   RCCreateRenderTarget( RenderTarget& rt ) : m_rt( rt ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command that sets a render target for rendering.
 */
class RCActivateRenderTarget  : public RenderCommand
{
private:
   RenderTarget*    m_renderTarget;

public:
   /**
    * Constructor.
    *
    * @param renderTarget     ptr to the render target ( NULL value also accepted )
    */
   RCActivateRenderTarget( RenderTarget* renderTarget );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command that allows you to query for the specified pixel color.
 */
class RCGetPixel : public RenderCommand
{
private:
   RenderTarget&        m_renderTarget;
   D3DXVECTOR2          m_queryPos;
   Color&               m_outColorVal;

public:
   /**
    * Constructor.
    *
    * @param renderTarget        queried render target
    * @param queryPos            pixel position
    * @param outColorVal         this is where the queried color value will be stored
    */
   RCGetPixel( RenderTarget& renderTarget, const D3DXVECTOR2& queryPos, Color& outColorVal );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};