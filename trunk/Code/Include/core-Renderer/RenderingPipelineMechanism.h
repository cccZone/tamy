/// @file   core-Renderer/RPSceneRenderPass.h
/// @brief  a mechanism responsible for rendering using a rendering pipeline
#pragma once

#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core-Renderer\RenderingMechanism.h"
#include "core\Observer.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class RenderingPipeline;
class Model;
class CameraContext;
class Camera;
class RenderingPipelineMechanismImpl;
class AttributeSorter;
class Renderer;
class RenderTarget;
class RenderTargetDescriptor;
class SpatialView;
class RenderingView;
class RenderingPipelineNode;
enum RenderingPipelineOperation;
enum RenderingPipelineNodeOperation;
class RuntimeDataBuffer;

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
class RenderingPipelineMechanism : public TRendererObject< RenderingPipelineMechanismImpl >,  
                                   public RenderingMechanism,
                                   public Observer< RenderingPipeline, RenderingPipelineOperation >,
                                   public Observer< RenderingPipelineNode, RenderingPipelineNodeOperation >
{
   DECLARE_RTTI_CLASS

private:
   /**
    * A helper structure that binds a rendered scene to the rendering mechanisms.
    */
   class RenderedScene
   {
   private:
      AttributeSorter*                             m_statesManager;
      SpatialView*                                 m_spatialView;
      RenderingView*                               m_renderingView;
      Model*                                       m_model;

   public:
      RenderedScene();
      ~RenderedScene();

      bool operator==( const Model& model ) const;

      void setModel( Model* model );

      void performVisibilityCheck( CameraContext& cameraContext );

      void render();
   };

private:
   RenderingPipeline*                           m_pipeline;

   Renderer*                                    m_renderer;
   std::vector< RenderedScene* >                m_scenes;
   CameraContext*                               m_cameraContext;

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
    * Sets a new active camera.
    *
    * @param camera
    */
   void setCamera( Camera& camera );

   /**
    * Checks if there's an active camera set.
    */
   inline bool hasActiveCamera() const { return m_cameraContext != NULL; }

   /**
    * Returns context of the currently used camera.
    */
   inline const CameraContext& getCameraContext() const { return *m_cameraContext; }

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

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void render();

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
    * Deinitializes the pipeline.
    */
   void deinitialize();
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pipeline mechanism implementation.
 */
class RenderingPipelineMechanismImpl : public RendererObjectImpl
{
public:
   virtual ~RenderingPipelineMechanismImpl() {}

   /**
    * Begins a rendering pass.
    */
   virtual void passBegin() {}

   /**
    * Ends a rendering pass.
    */
   virtual void passEnd() {}
};

///////////////////////////////////////////////////////////////////////////////
