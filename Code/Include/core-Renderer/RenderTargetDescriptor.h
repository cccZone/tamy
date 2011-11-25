/// @file   core-Renderer/RenderTargetDescriptor.h
/// @brief  a descriptor of a rendering target used by the rendering pipeline
#pragma once

#include "core/Object.h"
#include "core/Color.h"
#include "core-Renderer/ShaderTexture.h"
#include "core/RuntimeData.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A descriptor of a rendering target used by the rendering pipeline.
 */
class RenderTargetDescriptor : public Object
{
   DECLARE_CLASS( RenderTargetDescriptor )

private:
   std::string                   m_id;
   unsigned int                  m_initialWidth;
   unsigned int                  m_initialHeight;
   float                         m_widthScale;
   float                         m_heightScale;
   bool                          m_isDynamic;
   TextureUsage                  m_usage;
   bool                          m_isReadable;
   Color                         m_bgColor;

   // runtime data
   TRuntimeVar< RenderTarget* >  m_renderTarget;

public:
   /**
    * Constructor.
    */
   RenderTargetDescriptor();
   ~RenderTargetDescriptor();

   /**
    * Returns the render target's id.
    */
   inline const std::string& getTargetID() const { return m_id; }

   /**
    * Changes the target's ID.
    *
    * @param id
    */
   inline void setTargetID( const std::string& id ) { m_id = id; }

   /**
    * Tells if the render target has dynamic size.
    */
   inline bool isDynamic() const { return m_isDynamic; }

   /**
    * Sets the type of the render target.
    *
    * @param dynamic       defines whether the render target is dynamic or static
    */
   inline void setType( bool dynamic ) { m_isDynamic = dynamic; }

   /**
    * Returns texture usage.
    */
   inline TextureUsage getUsage() const { return m_usage; }

   /**
    * Sets the target usage.
    *
    * @param usage
    */
   inline void setUsage( TextureUsage usage ) { m_usage = usage; }

   /**
    * Tells if the render target is readable.
    */
   inline bool isReadable() const { return m_isReadable; }

   /**
    * Enables/disables reading ability of the render target.
    *
    * @param readable
    */
   inline void setReadable( bool readable ) { m_isReadable = readable; }

   /**
    * Returns the background color of the target.
    */
   inline const Color& getBgColor() const { return m_bgColor; }

   /**
    * Sets the target's background color.
    *
    * @param bgColor
    */
   inline void setBgColor( const Color& bgColor ) { m_bgColor = bgColor; }

   /**
    * Returns the static size settings of the target.
    *
    * @param outWidth
    * @param outHeight
    */
   inline void getStaticSize( unsigned int& outWidth, unsigned int& outHeight ) { outWidth = m_initialWidth; outHeight = m_initialHeight; }

   /**
    * Changes the definition of the render target's static size.
    *
    * @param width
    * @param height
    */
   inline void setStaticSize( unsigned int width, unsigned int height ) { m_initialWidth = width; m_initialHeight = height; }

   /**
    * Returns the dynamic size settings of the target.
    *
    * @param outWidthScale
    * @param outHeightScale
    */
   inline void getDynamicSize( float& outWidthScale, float& outHeightScale ) { outWidthScale = m_widthScale; outHeightScale = m_heightScale; }

   /**
    * Changes the definition of the render target's dynamic size.
    *
    * @param widthScale
    * @param heightScale
    */
   inline void setDynamicSize( float widthScale, float heightScale ) { m_widthScale = widthScale; m_heightScale = heightScale; }

   /**
    * Initializes the render target descriptor.
    *
    * @param runtimeData      runtimeDataBuffer
    * @param renderer         renderer that will be using this render target
    */
   void initialize( RuntimeDataBuffer& runtimeData, Renderer& renderer ) const;

   /**
    * Deinitializes the descriptor.
    *
    * @param runtimeData      runtimeDataBuffer
    */
   void deinitialize( RuntimeDataBuffer& runtimeData ) const;

   /**
    * Returns the render target.
    *
    * @param runtimeData      runtimeDataBuffer
    */
   inline RenderTarget* getTarget( RuntimeDataBuffer& runtimeData ) { return runtimeData[m_renderTarget]; }
};

///////////////////////////////////////////////////////////////////////////////
