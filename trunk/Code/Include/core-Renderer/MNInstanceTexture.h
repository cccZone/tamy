/// @file   core-Renderer/MNInstanceTexture.h
/// @brief  a node that provides a texture set for the host material entity instance
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/MaterialEntity.h"


///////////////////////////////////////////////////////////////////////////////

class MSTextureOutput;
class Texture;

///////////////////////////////////////////////////////////////////////////////

class MNInstanceTexture : public MaterialNode
{
   DECLARE_CLASS( MNInstanceTexture )

private:
   // static data
   MaterialTextures     m_usage;

   // runtime data
   MSTextureOutput*     m_output;
 
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
