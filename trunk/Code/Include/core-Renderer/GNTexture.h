/// @file   core-Renderer/GNTexture.h
/// @brief  a node that provides a texture
#pragma once

#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

class GSTextureOutput;
class Texture;
class RenderableTexture;

///////////////////////////////////////////////////////////////////////////////

class GNTexture : public GeometryShaderNode
{
   DECLARE_ALLOCATOR( GNTexture, AM_ALIGNED_16 );
   DECLARE_CLASS()

private:
   // static data
   Texture*             m_texture;

   // runtime data
   GSTextureOutput*     m_output;
   RenderableTexture*   m_renderableTexture;

public:
   /**
    * Constructor.
    */
   GNTexture();
   ~GNTexture();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // GeometryShaderNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const GeometryEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
