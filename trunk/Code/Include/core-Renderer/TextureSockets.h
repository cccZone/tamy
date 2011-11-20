/// @file   core-Renderer/TextureSockets.h
/// @brief  rendering pipeline node sockets that pass textures
#pragma once

#include "core-Renderer/RenderingPipelineSockets.h"
#include "core/RuntimeData.h"


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
    *
    * @param data       runtime data buffer
    */
   ShaderTexture* getTexture( RuntimeDataBuffer& data ) const;

protected:
   bool canConnect( RPNodeOutput& output ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPTextureOutput : public RPNodeOutput
{
   DECLARE_CLASS( RPTextureOutput )

public:
   /**
    * Constructor.
    */
   RPTextureOutput( const std::string& name = "" ) : RPNodeOutput( name ) {}
   virtual ~RPTextureOutput() {}

   /**
    * Returns a texture the output is using.
    *
    * @param data       runtime data buffer
    */
   virtual ShaderTexture* getTexture( RuntimeDataBuffer& data ) const { return NULL; }
};

///////////////////////////////////////////////////////////////////////////////

class RPImageOutput : public RPTextureOutput
{
   DECLARE_CLASS( RPTextureOutput )

private:
   TRuntimeVar< ShaderTexture* >           m_texture;

public:
   /**
    * Constructor.
    */
   RPImageOutput( const std::string& name = "" );

   /**
    * Sets a texture the output should emit.
    *
    * @param data          runtime data buffer
    * @param texture
    */
   void setTexture( RuntimeDataBuffer& data, ShaderTexture* texture ) const;

   // -------------------------------------------------------------------------
   // RPTextureOutput implementation
   // -------------------------------------------------------------------------
   ShaderTexture* getTexture( RuntimeDataBuffer& data ) const;

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
   void initialize( RenderingPipelineMechanism& host ) const;
   void deinitialize( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////

class RPRenderTargetOutput : public RPTextureOutput
{
   DECLARE_CLASS( RPRenderTargetOutput )

private:
   bool                                m_useBackBuffer;
   std::string                         m_renderTargetId;

   TRuntimeVar< RenderTarget* >        m_renderTarget;

public:
   /**
    * Constructor.
    */
   RPRenderTargetOutput( const std::string& name = "" );

   /**
    * Sets a new render target id.
    *
    * @param renderTargetId
    */
   void setRenderTargetID( const std::string& renderTargetId );

   /**
    * Returns the render target set.
    *
    * @param data       runtime data buffer
    */
   RenderTarget* getRenderTarget( RuntimeDataBuffer& data ) const;

   // -------------------------------------------------------------------------
   // RPTextureOutput implementation
   // -------------------------------------------------------------------------
   ShaderTexture* getTexture( RuntimeDataBuffer& data ) const;

   // -------------------------------------------------------------------------
   // RPNodeOutput implementation
   // -------------------------------------------------------------------------
   void createLayout( RenderingPipelineMechanism& host ) const;
   void initialize( RenderingPipelineMechanism& host ) const;
   void deinitialize( RenderingPipelineMechanism& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
