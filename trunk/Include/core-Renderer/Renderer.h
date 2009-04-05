#pragma once

#include <d3dx9.h>
#include <list>
#include "core\Delegate.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class SceneManager;
class Node;
class GraphicalNode;
class Camera;
class Light;
class Renderer;
class RenderingProcessor;
class SkyBox;
class RenderingPass;

///////////////////////////////////////////////////////////////////////////////

#define INNER_CLASS(ClassName)                                       \
   class ClassName;                                                  \
   friend class ClassName;                                           \
   class ClassName

///////////////////////////////////////////////////////////////////////////////

/**
 * This class implements the engine's rendering strategy.
 * It needs to be implemented in order to actually work with
 * the graphical equipment (the implementation can be done using
 * DirectX, openGL etc. - we don't specify that here)
 */
class Renderer
{
private:
   
   /**
    * The renderer is a state machine - it needs to be initialized,
    * then it can render, and from time to time it can loose
    * the device which needs to be regained
    */
   class RendererState
   {
   public:
      virtual ~RendererState() {}

      virtual void render(Renderer& renderer) = 0;
   };

private:
   std::vector<RenderingPass*> m_renderingPasses;
   std::list<SceneManager*> m_sceneManagers;

   unsigned int m_viewportWidth;
   unsigned int m_viewportHeight;

   RendererState* m_initialState;
   RendererState* m_renderingState;
   RendererState* m_deviceLostState;
   RendererState* m_currentRendererState;

public:
   Renderer();
   virtual ~Renderer();

   /**
    * Renders the graphical representation of the scene on
    * a graphical device. 
    * The graphical device used is specified in the implementation
    * of this abstract class
    */
   void render();

   /**
    * Changes the size of the active viewport
    */
   void resizeViewport(unsigned int width, unsigned int height);

   /**
    * This method adds a new 2d rendering pass to the rendering pipeline
    */
   void addPass(RenderingPass* pass);

   /** 
    * Adds a scene to the rendering queue. A single rendering pass
    * can render multiple scenes (2D, 3D etc.) at once
    */
   void addSceneManager(SceneManager& manager);

   /**
    * The method removes a scene from the rendering queue
    */
   void removeSceneManager(SceneManager& manager);

protected:
   /**
    * This method will be called whenever the device needs
    * to be initialized (which will basically take place
    * when the device is first referenced, or when we loose
    * the device's focus for some reason and then reaquire it)
    *
    * It should perform all the required initialization to get
    * the device ready for rendering
    */
   virtual void initRenderer() = 0;

   /**
    * This method sets the implementation-dependant details of the used viewport
    */
   virtual void resetViewport(unsigned int width, unsigned int height) = 0;

   /**
    * This method is expected to return the number of lights that can be simultaneously
    * set on the rendering device
    */
   virtual UINT getMaxLightsCount() const = 0;

   /**
    * The method sets the view matrix for the rendering pass
    */
   virtual void setViewMatrix(const D3DXMATRIX& mtx) = 0;

   /**
    * The method sets the projection matrix for the rendering pass
    */
   virtual void setProjectionMatrix(const D3DXMATRIX& mtx) = 0;

   /**
    * The method opens up the rendering conduct
    */
   virtual void renderingBegin() = 0;

   /**
    * The method presents the rendering results on the front buffer
    */
   virtual void renderingEnd() = 0;

   /**
    * The method tells us whether we can render anything, 
    * or shouldn't we bother trying
    */
   virtual bool isGraphicsSystemReady() const = 0;

   /**
    * This method should do anything it can to reaquire the graphical device 
    * for us.
    */
   virtual void attemptToRecoverGraphicsSystem() = 0;

private:
   // ---------------- Renderer states ---------------

   INNER_CLASS(InitialState) : public RendererState
   {
   public:
      void render(Renderer& renderer);
   };

   INNER_CLASS(RenderingState) : public RendererState
   {
   public:
      void render(Renderer& renderer);
   };

   INNER_CLASS(DeviceLostState) : public RendererState
   {
   public:
      void render(Renderer& renderer);
   };

   // --------- Renderer states manipulation ---------
   void setInitialState();
   void setRenderingState();
   void setDeviceLostState();
};

///////////////////////////////////////////////////////////////////////////////
