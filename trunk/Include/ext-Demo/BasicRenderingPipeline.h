#pragma once

#include "core\SpatialStorage.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CompositeRenderingMechanism;
class Overlay;
class RenderingMechanism;
class Camera;
class SkyBoxStorage;
class StaticGeometryRenderable;
class RenderablesStorage;
class LightsStorage;
class RenderingTarget;
class GraphicalEntitiesFactory;
class Renderer;
class RenderableNode;
class RendererImpl;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

class RERCreator;

///////////////////////////////////////////////////////////////////////////////

/**
 * This creator creates a single-stage rendering pipeline
 * capable of rendering a background and a lit scene with static & dynamic
 * elements, adding a single overlay on top.
 */
class BasicRenderingPipeline
{
private:
   GraphicalEntitiesFactory& m_factory;
   Renderer& m_renderer;
   Camera& m_camera;
   RenderingTarget* m_screenTarget;

   SpatialStorage<StaticGeometryRenderable>* m_staticSceneStorage;
   SpatialStorage<RenderableNode>* m_dynamicSceneStorage;
   RenderablesStorage* m_skyBox;
   RenderablesStorage* m_staticScene;
   RenderablesStorage* m_dynamicScene;
   LightsStorage* m_lights;
   Overlay* m_overlay;

   std::string m_rendererImplFile;
   RERCreator* m_rendererImplCreator;

public:
   /**
    * Constructor.
    *
    * @param factory       factory that will create the graphical
    *                      implementations required to assemble a pipeline
    * @param renderer      rendeer that will employ our pipeline
    * @param camera        camera we're gonna use to render the scene
    */
   BasicRenderingPipeline(GraphicalEntitiesFactory& factory,
                          Renderer& renderer,
                          Camera& camera);
   ~BasicRenderingPipeline();

   void create();

   void setBackground(RenderablesStorage* bg);
   void setStaticScene(SpatialStorage<StaticGeometryRenderable>* stat);
   void setDynamicScene(SpatialStorage<RenderableNode>* dyn);
   void setLights(LightsStorage* lights);
   void setOverlay(Overlay* overlay);
   void setRendererImpl(const std::string& effectFileName, RERCreator* creator);

   /**
    * Updates the state of the rendering pipeline.
    *
    * @param timeElapsed   time that has elapsed since the last time this method
    *                      was called
    */
   void update(float timeElapsed);

private:
   RendererImpl* createRendererImpl();
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo
