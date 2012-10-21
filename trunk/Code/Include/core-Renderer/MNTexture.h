/// @file   core-Renderer/MNTexture.h
/// @brief  a node that provides a texture
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSTextureOutput;
class Texture;
class RenderableTexture;

///////////////////////////////////////////////////////////////////////////////

class MNTexture : public MaterialNode
{
   DECLARE_ALLOCATOR( MNTexture, AM_ALIGNED_16 );
   DECLARE_CLASS()

private:
   // static data
   Texture*             m_texture;

   // runtime data
   MSTextureOutput*     m_output;
   RenderableTexture*   m_renderableTexture;

public:
   /**
    * Constructor.
    */
   MNTexture();
   ~MNTexture();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
