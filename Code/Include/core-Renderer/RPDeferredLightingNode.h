/// @file   core-Renderer/RPDeferredLightingNode.h
/// @brief  a node that provides information about the scene lights
#pragma once

#include "core-Renderer/RenderingPipelineMechanism.h"
#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RenderTarget;
class PixelShader;
class RPTextureInput;
class RPTextureOutput;

///////////////////////////////////////////////////////////////////////////////

/**
 * A node that provides information about the scene lights visible from the active camera.
 *
 * <lights.todo> : !!!! Even though I could technically use other building blocks that I already
 * have at my disposal to build this functionality, I want to encapsulate it
 * in one concise node. 
 * Additionally - one thing's missing from the blocks, and that is the ability to render geometry 
 * other than what GeometryResources can contain. Lights are also geometrical figures,
 * but I need to put more data in their vertices there than just the positions and normals.
 * That's why I decided to first create a standalone node that will deal with this 
 * new thing, and once I have the whole thing working, I will make the pipeline
 * more generic in terms of the geometry vertex definition - I will need that extra
 * flexibility when it comes to particles rendering etc.
 *
 * But even when that is done, I doubt I will move away from having just one node that calculates
 * the scene lighting, even if it's just a facade for the functionality of a couple of other blocks.
 * I want to keep it simple from the user's perspective.
 *
 * Once that's done - revise this comment!!!!!
 */
class RPDeferredLightingNode : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   std::string                         m_renderTargetId;

   // runtime data
   PixelShader*                        m_shader;
   TRuntimeVar< RenderTarget* >        m_renderTarget;

   // sockets data
   RPTextureInput*                     m_depthBufferInput;
   RPTextureInput*                     m_normalBufferInput;
   RPTextureOutput*                    m_lightTexture;

public:
   /**
    * Constructor.
    */
   RPDeferredLightingNode();
   ~RPDeferredLightingNode();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( RenderingPipelineMechanism& host ) const;
   void onDestroyLayout( RenderingPipelineMechanism& host ) const;
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

private:
   void initializeSocketd();
};

///////////////////////////////////////////////////////////////////////////////
