/// @file   core-Renderer/MNInstanceTexture.h
/// @brief  a node that provides a texture set for the host material entity instance
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class MSTextureOutput;
class Texture;
class RenderableTexture;

///////////////////////////////////////////////////////////////////////////////

class MNInstanceTexture : public MaterialNode
{
   DECLARE_CLASS()

private:
   // static data
   MaterialTextures     m_usage;

   // runtime data
   MSTextureOutput*     m_output;
   RenderableTexture*   m_renderableTexture;

public:
   /**
    * Constructor.
    */
   MNInstanceTexture();
   ~MNInstanceTexture();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
