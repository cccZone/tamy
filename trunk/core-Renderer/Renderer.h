#pragma once

#include <d3dx9.h>
#include <list>
#include "RenderingCommand.h"


///////////////////////////////////////////////////////////////////////////////

class Node;
class GraphicalNode;
class Camera;
class Light;
class Renderer;
class RenderingProcessor;
class SkyBox;

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

      virtual void render(Renderer& renderer, Node& rootNode) = 0;
   };

private:
   RenderingProcessor* m_renderingProcessor;
   Camera* m_activeCamera;
   SkyBox* m_skyBox;
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
    * Renders the graphical representation of the nodes hierarchy on
    * a graphical device. 
    * The graphical device used is specified in the implementation
    * of this abstract class
    */
   void render(Node& rootNode);

   /**
    * Sets a camera used to view the rendered scene. Without an active camera
    * no rendering will take place
    */
   void setActiveCamera(Camera& camera);

   /**
    * In order for the rendered scene to appear more lifelike, we can
    * add a background. A skybox is a perfect choice - and this method allows to add it.
    */
   void setSkyBox(SkyBox& skyBox);

   /**
    * Changes the size of the active viewport
    */
   void resizeViewport(unsigned int width, unsigned int height);

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
    * The method executes the rendering commands and thus rendering stuff
    * on the graphical device.
    */
   virtual void executeRenderingCommands(const std::list<RenderingCommand>& commands) = 0;

   /**
    * The method renders whatever's there in the queue and clear it afterwards.
    */
   virtual void present() = 0;

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
   void resetActiveCameraSettings();
   void addBackgroundRenderingCommand(std::list<RenderingCommand>& renderingCommands);

private:
   // ---------------- Renderer states ---------------

   INNER_CLASS(InitialState) : public RendererState
   {
   public:
      void render(Renderer& renderer, Node& rootNode);
   };

   INNER_CLASS(RenderingState) : public RendererState
   {
   public:
      void render(Renderer& renderer, Node& rootNode);
   };

   INNER_CLASS(DeviceLostState) : public RendererState
   {
   public:
      void render(Renderer& renderer, Node& rootNode);
   };

   // --------- Renderer states manipulation ---------
   void setInitialState();
   void setRenderingState();
   void setDeviceLostState();
};

///////////////////////////////////////////////////////////////////////////////
