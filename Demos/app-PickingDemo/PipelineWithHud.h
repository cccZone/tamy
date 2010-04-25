#pragma once

#include "core\SpatialStorage.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CompositeRenderingMechanism;
class Overlay;
class RenderingMechanism;
class Camera;
class RenderablesStorage;
class LightsStorage;
class RenderingTarget;
class GraphicalEntitiesFactory;
class Renderer;
class RenderableNode;

///////////////////////////////////////////////////////////////////////////////

class PipelineWithHud
{
private:
   GraphicalEntitiesFactory& m_factory;
   Renderer& m_renderer;
   Camera& m_regularCamera;
   Camera& m_hudCamera;
   RenderingTarget* m_screenTarget;

   SpatialStorage<RenderableNode>* m_regularSceneStorage;
   SpatialStorage<RenderableNode>* m_hudSceneStorage;
   RenderablesStorage* m_regularScene;
   RenderablesStorage* m_hudScene;
   LightsStorage* m_regularLights;
   LightsStorage* m_hudLights;
   Overlay* m_overlay;

public:
   /**
    * Constructor.
    *
    * @param factory       factory that will create the graphical
    *                      implementations required to assemble a pipeline
    * @param renderer      rendeer that will employ our pipeline
    * @param regularCamera camera we're gonna use to render the scene
    * @param hudCamera     camera we're gonna use to render the hud
    */
   PipelineWithHud(GraphicalEntitiesFactory& factory,
                   Renderer& renderer,
                   Camera& regularCamera,
                   Camera& hudCamera);
   ~PipelineWithHud();

   void create();

   void setRegularScene(SpatialStorage<RenderableNode>* scene);
   void setHudScene(SpatialStorage<RenderableNode>* scene);
   void setRegularLights(LightsStorage* lights);
   void setHudLights(LightsStorage* lights);
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
