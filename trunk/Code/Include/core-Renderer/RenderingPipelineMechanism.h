/// @file   core-Renderer/RPSceneRenderPass.h
/// @brief  a mechanism responsible for rendering using a rendering pipeline
#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderCommand.h"
#include "core\IDebugDraw.h"
#include "core\Observer.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;
class Model;
class Camera;
class Renderer;
class RenderTarget;
class RenderTargetDescriptor;
class RenderingView;
class RenderingPipelineNode;
enum RenderingPipelineOperation;
enum RenderingPipelineNodeOperation;
class RuntimeDataBuffer;
class ModelDebugScene;
class ModelView;

///////////////////////////////////////////////////////////////////////////////

/**
 * Scenes a rendering pipeline can render ( contains a list of all scenes 
 * we can possibly register and operate on ).
 */
enum RPMSceneId
{
   RPS_Main,
   RPS_Debug,
   RPS_MaxScenes
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A pass responsible for rendering using a rendering pipeline.
 */
class RenderingPipelineMechanism : public RenderingMechanism,
                                   public Observer< RenderingPipeline, RenderingPipelineOperation >,
                                   public Observer< RenderingPipelineNode, RenderingPipelineNodeOperation >
{
private:
   /**
    * A helper structure that binds a rendered scene to the rendering mechanisms.
    */
   class RenderedScene
   {
   private:
      RenderingView*                               m_renderingView;
      ModelDebugScene*                             m_debugSceneView;

      Model*                                       m_model;

   public:
      RenderedScene();
      ~RenderedScene();

      void initialize( Renderer& renderer );

      bool operator==( const Model& model ) const;

      void setModel( Model* model );

      void setDebugScene( DebugScene& scene );

      void render();
   };

private:
   RenderingPipeline*                           m_pipeline;

   Renderer*                                    m_renderer;
   std::vector< RenderedScene* >                m_scenes;

   DebugScene*                                  m_debugScene;

   std::vector< RenderingPipelineNode* >        m_nodesQueue;

   // a buffer for storing runtime data the pipeline will operate on
   RuntimeDataBuffer*                           m_runtimeDataBuffer;
 
public:
   /**
    * Constructor.
    *
    * @param pipeline         rendering pipeline
    */
   RenderingPipelineMechanism( RenderingPipeline* pipeline = NULL );
   ~RenderingPipelineMechanism();

   /**
    * Returns the currently used renderer instance.
    */
   inline Renderer& getRenderer() { return *m_renderer; }

   /**
    * Gives access to a runtime data buffer.
    */
   inline RuntimeDataBuffer& data() { return *m_runtimeDataBuffer; }

   // -------------------------------------------------------------------------
   // Params management
   // -------------------------------------------------------------------------
   /**
    * Registers a new scene for rendering.
    *
    * @param scene
    */
   void addScene( RPMSceneId sceneId, Model& scene );

   /**
    * Removes the specified scene from rendering.
    *
    * @param scene
    */
   void removeScene( Model& scene );

   /**
    * Removes a scene registered under the specified sceneId from rendering.
    *
    * @param sceneId
    */
   void removeScene( RPMSceneId sceneId );

   /**
    * Sets a debug scene instance.
    *
    * @param debug scene
    */
   void setDebugScene( DebugScene& debugScene );

   /**
    * Returns a render target registered under the specified ID.
    *
    * @param id         render target id
    */
   RenderTarget& getRenderTarget( const std::string& id ) const;

   /**
    * Renders a scene with the specified id on the specified render target
    *
    * @param sceneId
    * @param renderTarget
    */
   void renderScene( RPMSceneId sceneId, RenderTarget* renderTarget ) const;

   /**
    * Renders the debug scene on the specified render target.
    *
    * @param renderTarget
    */
   void renderDebugScene( RenderTarget* renderTarget );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update( RenderingPipeline& subject );
   void update( RenderingPipeline& subject, const RenderingPipelineOperation& msg );
   void update( RenderingPipelineNode& subject );
   void update( RenderingPipelineNode& subject, const RenderingPipelineNodeOperation& msg );

private:
   /**
    * Caches the nodes for quicker processing.
    */
   void cacheNodes();

   /**
    * Draws a debug grid.
    */
   void drawGrid() const;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that begins the process of rendering a scene.
 */
class RCBeginScene : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that ends the process of rendering a scene.
 */
class RCEndScene : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////