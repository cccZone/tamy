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
 * A special kind of texture which a renderer uses as an output.
 */
class RenderTarget : public TRendererObject< RenderTargetImpl >, 
                     public UniqueObject,
                     public ShaderTexture,
                     public Observer< Renderer, RendererOps >
{
   DECLARE_RTTI_CLASS

private:
   Renderer*      m_renderer;

   unsigned int   m_width;
   unsigned int   m_height;
   bool           m_isReadable;
   Color          m_bgColor;

public:
   /**
    * Constructor.
    *
    * @param renderer
    * @param isReadable    tells if we can perform reading operations on the texture
    */
   RenderTarget( Renderer* renderer = NULL, bool isReadable = false, const Color& bgColor = Color( 0.f, 0.f, 0.f, 0.f ) );
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
   // Renderer observer implementation
   // -------------------------------------------------------------------------
   void update(Renderer& renderer);
   void update(Renderer& renderer, const RendererOps& operation);
};

///////////////////////////////////////////////////////////////////////////////
