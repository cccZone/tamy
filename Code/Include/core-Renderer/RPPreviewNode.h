/// @file   core-Renderer/RPPreviewNode.h
/// @brief  a node that allows to preview the contents of a render target.
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class RPTextureInput;

///////////////////////////////////////////////////////////////////////////////

enum PreviewType
{
   PT_Color,
   PT_Depth,
   PT_Normals,
   PT_Specular
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A post-process effect that blurs an image.
 */
class RPPreviewNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   PreviewType          m_type;

   // runtime data
   PixelShader*         m_shader;

   // sockets
   RPTextureInput*      m_texInput;

public:
   /**
    * Constructor.
    */
   RPPreviewNode();

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
