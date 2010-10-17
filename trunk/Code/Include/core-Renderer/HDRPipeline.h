/// @file   core-Renderer/HDRPipeline.h
/// @brief  an HDR rendering mechanism
#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Model;
class SceneRenderingPass;
class VisibilityPass;
class PixelShader;
class RenderTarget;
class FullscreenQuad;
class Camera;
class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * Sn HDR rendering mechanism.
 */
class HDRPipeline : public RenderingMechanism
{
private:
   ResourcesManager&                   m_rm;
   Camera&                             m_camera;

   Renderer*                           m_renderer;

   RenderTarget*                       m_sceneRenderTarget;
   RenderTarget*                       m_brightPassTarget;
   RenderTarget*                       m_downSampleTarget;
   RenderTarget*                       m_horizBlurTarget;
   RenderTarget*                       m_vertBlurTarget;
   std::vector< RenderTarget* >        m_luminanceTarget;

   VisibilityPass*                     m_visibilityPass;
   SceneRenderingPass*                 m_scenePass;
   PixelShader*                        m_brightPass;
   PixelShader*                        m_downSamplePass;
   PixelShader*                        m_horizBlurPass;
   PixelShader*                        m_vertBlurPass;
   PixelShader*                        m_greyscalePass;
   PixelShader*                        m_luminanceDownSamplePass;
   PixelShader*                        m_hdrFinalPass;

   FullscreenQuad*                     m_fullscreenQuad;

   // parameters
   float                               m_brightThreshold;
   float                               m_gaussMultiplier;
   float                               m_gaussMean;
   float                               m_gaussStdDev;
   float                               m_exposure;
   unsigned int                        m_luminanceStepsCount;

public:
   /**
    * Constructor.
    */
   HDRPipeline( ResourcesManager& rm, Camera& camera );
   ~HDRPipeline();

   /**
    * Adds a new scene to the renderer.
    *
    * @param scene
    */
   void addScene( Model& scene );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void render();

private:
   void renderScene();
   void measureLuminance();
   void renderPostprocess();
   void renderHDR();

   float computeGaussianValue( float x, float mean, float std_deviation ) const;
};

///////////////////////////////////////////////////////////////////////////////
