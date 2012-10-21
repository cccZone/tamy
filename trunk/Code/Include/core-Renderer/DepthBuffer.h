/// @file   core-Renderer/DepthBuffer.h
/// @brief  custom depth buffer that can be set instead of the default one to improve rendering resolution
#pragma once

#include "core-Renderer\RenderResource.h"
#include "core-Renderer\RenderCommand.h"
#include "core\types.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Custom depth buffer that can be set instead of the default one to improve rendering resolution.
 */
class DepthBuffer : public RenderResource
{
   DECLARE_ALLOCATOR( DepthBuffer, AM_DEFAULT );

private:
   uint     m_width;
   uint     m_height;

public:
   /**
    * Constructor.
    *
    * @param width
    * @param height
    */
   DepthBuffer( uint width, uint height );
   ~DepthBuffer();

   /**
    * Returns buffer dimensions.
    *
    * @param outWidth
    * @param outHeight
    */
   inline void getDimensions( uint &outWidth, uint& outHeight ) const { outWidth = m_width; outHeight = m_height; }

   /**
    * Sets new buffer dimensions.
    *
    * @param width
    * @param height
    */
   inline void setDimensions( uint width, uint height ) { m_width = width; m_height = height; setDirty(); }
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command that sets a depth buffer up for rendering.
 */
class RCActivateDepthBuffer  : public RenderCommand
{
   DECLARE_ALLOCATOR( RCActivateDepthBuffer, AM_DEFAULT );

private:
   DepthBuffer*     m_depthBuffer;

public:
   /**
    * Constructor.
    *
    * @param depthBuffer     ptr to the depth buffer
    */
   RCActivateDepthBuffer( DepthBuffer* depthBuffer );

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command that unbinds a depth buffer disabling any further rendering to it.
 */
class RCDeactivateDepthBuffer  : public RenderCommand
{
   DECLARE_ALLOCATOR( RCDeactivateDepthBuffer, AM_DEFAULT );

private:
   DepthBuffer*     m_depthBuffer;

public:
   /**
    * Constructor.
    *
    * @param depthBuffer     ptr to the depth buffer
    */
   RCDeactivateDepthBuffer( DepthBuffer* depthBuffer );
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
