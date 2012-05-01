/// @file   core-Renderer/RPTextureNode.h
/// @brief  a node that provides a texture
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPTextureOutput;
class Texture;
class RenderableTexture;

///////////////////////////////////////////////////////////////////////////////

class RPTextureNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   Texture*             m_texture;

   // runtime data
   RPTextureOutput*     m_output;
   RenderableTexture*   m_renderableTexture;

public:
   /**
    * Constructor.
    */
   RPTextureNode();
   ~RPTextureNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
