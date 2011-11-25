/// @file   core-Renderer/MNTexture.h
/// @brief  a node that provides a texture
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSTextureOutput;
class Texture;

///////////////////////////////////////////////////////////////////////////////

class MNTexture : public MaterialNode
{
   DECLARE_CLASS( MNTexture )

private:
   // static data
   Texture*             m_texture;

   // runtime data
   MSTextureOutput*     m_output;
 
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
};

///////////////////////////////////////////////////////////////////////////////
