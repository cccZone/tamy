/// @file   core-Renderer/RPSceneRenderPass.h
/// @brief  a mechanism responsible for rendering using a rendering pipeline
#pragma once

#include "core-Renderer\RendererObject.h"
#include "core-Renderer\RendererObjectImpl.h"
#include "core-Renderer\RenderingMechanism.h"
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

///////////////////////////////////////////////////////////////////////////////

/**
 * A pass responsible for rendering using a rendering pipeline.
 */
class RenderingPipelineMechanism : public TRendererObject< RenderingPipelineMechanismImpl >,  
                                   public RenderingMechanism
{
   DECLARE_RTTI_CLASS

private:
   RenderingPipeline*                           m_pipeline;

   Renderer*                                    m_renderer;
   std::vector< Model* >                        m_scenes;
   CameraContext*                               m_cameraContext;

   std::vector< RenderingPipelineNode* >        m_nodesQueue;

   // we're using a single camera and a some scenes - but all are defined here - so render them before 
   // the pipeline
   AttributeSorter*                             m_statesManager;
   SpatialView*                                 m_spatialView;
   RenderingView*                               m_renderingView;
 
public:
   /**
    * Constructor.
    *
    * @param pipeline         rendering pipeline
    */
   RenderingPipelineMechanism( RenderingPipeline* pipeline = NULL );
   ~RenderingPipelineMechanism();

   /**
    * Returns instance of the states manager this pass uses.
    */
   inline AttributeSorter& getStatesMgr() { return *m_statesManager; }

   /**
    * Returns the currently used renderer instance.
    */
   inline Renderer& getRenderer() { return *m_renderer; }

   // -------------------------------------------------------------------------
   // Params management
   // -------------------------------------------------------------------------
   /**
    * Registers a new scene for rendering.
    *
    * @param scene
    */
   void addScene( Model& scene );

   /**
    * Removes a scene from rendering.
    *
    * @param scene
    */
   void removeScene( Model& scene );

   /**
    * Returns an array of scenes the pass is set up with.
    */
   inline const std::vector< Model* >& getScenes() const { return m_scenes; }

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

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void render();

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
