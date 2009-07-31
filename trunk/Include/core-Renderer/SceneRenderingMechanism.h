#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include "core\Array.h"
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class VisualSceneManager;
class Camera;
class RenderingProcessor;
class GraphicalNodesProcessor;
class AbstractGraphicalNode;
class MaterialImpl;

///////////////////////////////////////////////////////////////////////////////

class SceneRenderingMechanism : public RenderingMechanism
{
private:
   unsigned int m_maxLightsCount;
   MaterialImpl* m_impl;

   Array<VisualSceneManager*> m_sceneManagers;

   // scene rendering helpers
   GraphicalNodesProcessor* m_nodesProcessor;
   RenderingProcessor* m_renderingProcessor;

   Array<AbstractGraphicalNode*> m_visibleNodes;
   Array<AbstractGraphicalNode*> m_nodesToProcess;

public:
   SceneRenderingMechanism(RenderingTargetsPolicy* policy,
                           unsigned int maxLightsCount,
                           MaterialImpl* impl);
   ~SceneRenderingMechanism();

   void render();

   /** 
    * Adds a scene to the rendering queue. A single rendering pass
    * can render multiple scenes (2D, 3D etc.) at once
    */
   void addVisualSceneManager(VisualSceneManager& manager);

   /**
    * The method removes a scene from the rendering queue
    */
   void removeVisualSceneManager(VisualSceneManager& manager);

protected:
      /**
    * The method sets the view matrix for the rendering pass
    */
   virtual void setViewMatrix(const D3DXMATRIX& mtx) = 0;

   /**
    * The method sets the projection matrix for the rendering pass
    */
   virtual void setProjectionMatrix(const D3DXMATRIX& mtx) = 0;

private:
   void renderBackground(VisualSceneManager& sceneManager);
   void renderContents(VisualSceneManager& sceneManager, Camera& activeCamera);

};

///////////////////////////////////////////////////////////////////////////////
