#pragma once

#include <d3dx9.h>
#include <list>
#include "core\Array.h"
#include "core\Subject.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct Point;
class RendererObserver;
class RenderingMechanism;
class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

#define INNER_CLASS(ClassName)                                       \
   class ClassName;                                                  \
   friend class ClassName;                                           \
   class ClassName

///////////////////////////////////////////////////////////////////////////////

enum RendererOps
{
   RO_RESIZE_VIEWPORT
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class implements the engine's rendering strategy.
 * It needs to be implemented in order to actually work with
 * the graphical equipment (the implementation can be done using
 * DirectX, openGL etc. - we don't specify that here)
 *
 * The renderer can be observer - it will notify it's observers about the
 * changes in its state (rendering, device lost, device recovered)
 */
class Renderer : public Subject<Renderer, RendererOps>
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
   RenderingMechanism* m_mechanism;

   unsigned int m_viewportWidth;
   unsigned int m_viewportHeight;
   unsigned int m_leftClientArea;
   unsigned int m_topClientArea;
   unsigned int m_rightClientArea;
   unsigned int m_bottomClientArea;

   RendererState* m_initialState;
   RendererState* m_renderingState;
   RendererState* m_deviceLostState;
   RendererState* m_currentRendererState;

public:
   Renderer(unsigned int viewportWidth = 800,
            unsigned int viewportHeight = 600);
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
   void resizeViewport(unsigned int width, unsigned int height,
                       unsigned int leftClientArea, unsigned int topClientArea,
                       unsigned int rightClientArea, unsigned int bottomClientArea);

   /**
    * The method translates the screen space coordinates
    * (i.e. ones your mouse moves in) to viewport space coordinates
    */
   void screenToViewport(const Point& inScreenPt, D3DXVECTOR2& outViewportPt) const;

   /**
    * This method set a new rendering mechanism.
    *
    * @param mech    new mechanism that will be used for rendering.
    */
   void setMechanism(RenderingMechanism* mech);

   /**
    * This method returns the width of currently set viewport
    */
   unsigned int getViewportWidth() const {return m_viewportWidth;}

   /**
    * This method returns the height of currently set viewport
    */
   unsigned int getViewportHeight() const {return m_viewportHeight;}

protected:
   /**
    * This method sets the implementation-dependent details of the used viewport
    *
    * When it's called, the device needs to be initialized (which will basically take place
    * when the device is first referenced, or when we loose
    * the device's focus for some reason and then reacquire it)
    *
    * It should perform all the required initialization to get
    * the device ready for rendering
    */
   virtual void resetViewport(unsigned int width, unsigned int height) = 0;

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
