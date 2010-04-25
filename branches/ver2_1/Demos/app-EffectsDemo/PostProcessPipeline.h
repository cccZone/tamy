#pragma once

#include "core\SpatialStorage.h"
#include "ext-Demo\DemoApp.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CompositeRenderingMechanism;
class Overlay;
class RenderingMechanism;
class Camera;
class SkyBoxStorage;
class RenderablesStorage;
class LightsStorage;
class RenderingTarget;
class GraphicalEntitiesFactory;
class Renderer;
class RenderableNode;
class RendererImpl;
class PostProcessEffectRenderable;
class TextureRenderingTarget;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{
   class RERCreator;
}

///////////////////////////////////////////////////////////////////////////////

class PostProcessPipeline
{
private:
   GraphicalEntitiesFactory& m_factory;
   Renderer& m_renderer;
   Camera& m_camera;
   TextureRenderingTarget* m_middleTarget;
   RenderingTarget* m_screenTarget;

   demo::StaticSceneManager* m_staticSceneStorage;
   SpatialStorage<RenderableNode>* m_dynamicSceneStorage;
   RenderablesStorage* m_skyBox;
   RenderablesStorage* m_staticScene;
   RenderablesStorage* m_dynamicScene;
   RenderablesStorage* m_postProcessScene;
   LightsStorage* m_lights;
   Overlay* m_overlay;

   std::string m_rendererImplFile;
   demo::RERCreator* m_rendererImplCreator;

public:
   /**
    * Constructor.
    *
    * @param factory       factory that will create the graphical
    *                      implementations required to assemble a pipeline
    * @param renderer      rendeer that will employ our pipeline
    * @param camera        camera we're gonna use to render the scene
    */
   PostProcessPipeline(GraphicalEntitiesFactory& factory,
                       Renderer& renderer,
                       Camera& camera);
   ~PostProcessPipeline();

   void create();

   void setBackground(RenderablesStorage* bg);
   void setStaticScene(demo::StaticSceneManager* stat);
   void setDynamicScene(SpatialStorage<RenderableNode>* dyn);
   void setPostProcessScene(PostProcessEffectRenderable* ppRen);
   void setRendererImpl(const std::string& effectFileName, demo::RERCreator* creator);
   void setLights(LightsStorage* lights);
   void setOverlay(Overlay* overlay);

   /**
    * Updates the state of the rendering pipeline.
    *
    * @param timeElapsed   time that has elapsed since the last time this method
    *                      was called
    */
   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////
