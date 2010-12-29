#pragma once

/// @file   core-Renderer\RenderTarget.h
/// @brief  a special kind of texture which a renderer uses as an output

#include "core\UniqueObject.h"
#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core\Observer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\Color.h"
#include <string>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class Renderer;
enum RendererOps;
class RenderTargetImpl;
class RenderTargetSizePolicy;
enum TextureFormat;

///////////////////////////////////////////////////////////////////////////////

/**
 * A special kind of texture which a renderer uses as an output.
 */
class RenderTarget : public TRendererObject< RenderTargetImpl >, 
                     public UniqueObject,
                     public ShaderTexture
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
    * Returns a pointer to the platform specific implementation
    * of the texture.
    *
    * Only the implementation will know what to do with it.
    *
    * @return     pointer to impl-specific texture structure
    */
   void* getPlatformSpecific() const;

   /**
    * Returns the width of the texture.
    */
   inline unsigned int getWidth() const { return m_width; }

   /**
    * Returns the height of the texture.
    */
   inline unsigned int getHeight() const { return m_height; }

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
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Library dependent texture implementation.
 */
class RenderTargetImpl : public RendererObjectImpl
{
public:
   virtual ~RenderTargetImpl() {}

   /**
    * Should return a pointer to the library-specific render target representation.
    *
    * @return     pointer to impl-specific render target structure
    */
   virtual void* getPlatformSpecific() const { return NULL; }

   /**
    * Should return the color of the specified pixel.
    *
    * @param pos     pixel position
    */
   virtual Color getPixel( const D3DXVECTOR2& pos ) const { return Color(); }
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
