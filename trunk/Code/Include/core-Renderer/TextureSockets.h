/// @file   core-Renderer/TextureSockets.h
/// @brief  rendering pipeline node sockets that pass textures
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class ShaderTexture;

///////////////////////////////////////////////////////////////////////////////

class RPTextureInput : public RPNodeInput
{
   DECLARE_CLASS( RPTextureInput )

public:
   /**
    * Constructor.
    */
   RPTextureInput( const std::string& name = "" );

   /**
    * Returns the texture exposed by the output the socket's connected to.
    */
   ShaderTexture* getTexture();

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPTextureOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPTextureOutput )

private:
   bool              m_useBackBuffer;
   std::string       m_renderTargetId;

   RenderTarget*     m_renderTarget;

public:
   /**
    * Constructor.
    */
   RPTextureOutput( const std::string& name = "" );

   /**
    * Sets a new render target id.
    *
    * @param renderTargetId
    */
   inline void setRenderTargetID( const std::string renderTargetId ) { m_renderTargetId = renderTargetId; m_useBackBuffer = m_renderTargetId.empty(); }

   /**
    * Returns the render target set.
    */
   inline RenderTarget* getRenderTarget() { return m_renderTarget; }

   /**
    * Returns a texture the output is using.
    */
   ShaderTexture* getTexture() const;

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void initialize( RenderingPipelineMechanism& host );
   void deinitialize( RenderingPipelineMechanism& host );
};

///////////////////////////////////////////////////////////////////////////////
