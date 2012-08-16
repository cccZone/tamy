/// @file   core-Renderer/RPSceneRenderPass.h
/// @brief  a mechanism responsible for rendering using a rendering pipeline
#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderCommand.h"
#include "core\Observer.h"
#include "core\Array.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;
class Model;
class Camera;
class Renderer;
class RenderTarget;
class RenderTargetDescriptor;
class DepthBuffer;
class RenderingView;
class RenderingPipelineNode;
enum GraphBuilderOperation;
enum GraphBuilderNodeOperation;
class RuntimeDataBuffer;
class ModelDebugScene;
class ModelView;
class Geometry;
class Light;


///////////////////////////////////////////////////////////////////////////////

/**
 * A pass responsible for rendering using a rendering pipeline.
 */
class RenderingPipelineMechanism : public RenderingMechanism,
                                   public Observer< RenderingPipeline, GraphBuilderOperation >,
                                   public Observer< RenderingPipelineNode, GraphBuilderNodeOperation >
{
private:
   /**
    * A helper structure that binds a rendered scene to the rendering mechanisms.
    */
   class RenderedScene
   {
   public:
      Array< Geometry* >                           m_visibleElems;
      Array< Light* >                              m_visibleLights;

      RenderingView*                               m_renderingView;

    private:
      Model*                                       m_model;

   public:
      RenderedScene();
      ~RenderedScene();

      void initialize( Renderer& renderer );

      void deinitialize( Renderer& renderer );

      bool operator==( const Model& model ) const;

      void setModel( Model* model );

      void queryVisibleElements();

   };

private:
   RenderingPipeline*                           m_pipeline;

   Renderer*                                    m_renderer;
   RenderedScene*                               m_scene;

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
    * Registers a new scene for rendering, or unregisters it, if NULL instance is passed
    *
    * @param scene
    */
   void setScene( Model* scene );

   /**
    * Returns the visible elements from the scene
    *
    */
   const Array< Geometry*> & getSceneElements() const;

   /**
    * Returns visible scene lights.
    */
   const Array< Light*> & getSceneLights() const;

   /**
    *
    */
   const RenderingView* getSceneRenderingView() const;

   /**
    * Checks if a render target registered with the specified ID exists.
    *
    * @param id         render target id
    */
   bool isRenderTargetDefined( const std::string& id ) const;

   /**
    * Returns a render target registered under the specified ID.
    *
    * @param id         render target id
    */
   RenderTarget* getRenderTarget( const std::string& id ) const;

   /**
    * Returns a depth buffer registered under the specified ID.
    *
    * @param id         depth buffer id
    */
   DepthBuffer* getDepthBuffer( const std::string& id ) const;

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
   void update( RenderingPipeline& subject, const GraphBuilderOperation& msg );
   void update( RenderingPipelineNode& subject );
   void update( RenderingPipelineNode& subject, const GraphBuilderNodeOperation& msg );

private:
   /**
    * Caches the nodes for quicker processing.
    */
   void cacheNodes();

   void pipelineInitialization();
   void pipelineDeinitialization();
};

///////////////////////////////////////////////////////////////////////////////
