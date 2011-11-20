/// @file   core-Renderer/RPTextureNode.h
/// @brief  a node that provides a texture
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPImageOutput;
class Texture;

///////////////////////////////////////////////////////////////////////////////

class RPTextureNode : public RenderingPipelineNode
{
   DECLARE_CLASS( RPTextureNode )

private:
   // static data
   Texture*          m_texture;

   // runtime data
   RPImageOutput*    m_output;
 
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
