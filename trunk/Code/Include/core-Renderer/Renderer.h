#ifndef _RENDERER_H
#define _RENDERER_H

#include <list>
#include "core\Array.h"
#include "core\Subject.h"
#include "core\UniqueObject.h"
#include "core\Color.h"
#include "core\RoundBuffer.h"
#include "core-AppFlow\TimeDependent.h"
#include "core\types.h"
#include "core\Matrix.h"
#include "core\Stack.h"
#include <vector>
#include <map>
#include <set>


///////////////////////////////////////////////////////////////////////////////

struct Point;
class RendererObserver;
class RenderingMechanism;
class RenderingTargetsPolicy;
class RenderTarget;
class Camera;
class RenderCommand;

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
class Renderer : public Subject< Renderer, RendererOps >, public UniqueObject< Renderer >, public TimeDependent
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

      virtual void render( Renderer& renderer ) = 0;
   };

   typedef std::set< RenderTarget* >   RenderTargetsList;

private:
   // rendering mechanism
   RenderingMechanism*              m_mechanism;

   // camera
   Camera*                          m_defaultCamera;
   Stack< Camera* >                 m_camerasStack;

   // viewport
   unsigned int                     m_viewportWidth;
   unsigned int                     m_viewportHeight;
   unsigned int                     m_leftClientArea;
   unsigned int                     m_topClientArea;
   unsigned int                     m_rightClientArea;
   unsigned int                     m_bottomClientArea;
   Matrix                           m_viewportMatrix;

   // internal state machine
   RendererState*                   m_initialState;
   RendererState*                   m_renderingState;
   RendererState*                   m_deviceLostState;
   RendererState*                   m_currentRendererState;

   // render targets
   RenderTargetsList                m_renderTargetsList;

   // render commands
   RoundBuffer                      m_renderCommands;

   // shaders support
   uint                             m_vertexShaderTechnique;

public:
   Renderer( uint viewportWidth = 800, uint viewportHeight = 600 );
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

   /**
    * Returns current viewport matrix.
    */
   inline const Matrix& getViewportMtx() const { return m_viewportMatrix; }

   /**
    * A command for setting a render target.
    *
    * Targets are scoped during rendering - each target will be cleaned
    * only the first time it's set during a rendering frame - that's
    * why we need this centralized command that operates in the context
    * of a rendering frame, which is managed by the main renderer.
    *
    * @param renderTarget     a pointer to a rendering target, or NULL
    *                         if we want to use the back buffer.
    * @param targetIdx        target index
    */
   void bindRenderTarget( RenderTarget* renderTarget, uint targetIdx );

   /**
    * Unbinds the specified render target, disabling any further rendering to it.
    *
    * @param targetIdx
    */
   void unbindRenderTarget( uint targetIdx );

   // ----------------------------------------------------------------------------
   // Camera management
   // ----------------------------------------------------------------------------
   /**
    * Returns the currently active camera ( const version )
    */
   inline const Camera& getActiveCamera() const { return *m_camerasStack.top(); }

   /**
    * Returns the currently active camera.
    */
   inline Camera& getActiveCamera() { return *m_camerasStack.top(); }

   /**
    * Pushes a camera to the top of the camera stack, thus making it an active camera for this renderer.
    *
    * @param camera
    */
   void pushCamera( Camera& camera );

   /**
    * Pops a camera from the top of the stack, deactivating it.
    * The default camera will never be popped - there always needs to be at least one active camera remaining.
    */
   void popCamera();

   // ----------------------------------------------------------------------------
   // Shaders API
   // ----------------------------------------------------------------------------

   /**
    * Sets an id of a vertex shader technique the vertex shaders from this point on should use
    */
   inline void setVertexShaderTechnique( uint techniqueId ) { m_vertexShaderTechnique = techniqueId; }

   /**
    * Resets an id of a vertex shader technique the vertex shaders from this point on should use
    */
   inline void resetVertexShaderTechnique() { m_vertexShaderTechnique = 0; }

   /**
    * Returns the id of a vertex shader technique that should be used to render the geometry.
    */
   inline uint getVertexShaderTechnique() const { return m_vertexShaderTechnique; }

   // ----------------------------------------------------------------------------
   // Render commands queue
   // ----------------------------------------------------------------------------
   /**
    * Gives access to the commands buffer.
    */
   inline RoundBuffer& operator()() { return m_renderCommands; }

   // ----------------------------------------------------------------------------
   // TimeDependent implementation
   // ----------------------------------------------------------------------------
   inline void update( float timeDelta )
   {
      render();
   }

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
    * This method should do anything it can to reacquire the graphical device 
    * for us.
    */
   virtual void attemptToRecoverGraphicsSystem() = 0;

   // -------------------------------------------------------------------------
   // render targets management
   // -------------------------------------------------------------------------
   /**
    * This method should make the specified render target an active rendering
    * target on the underlying rendering device.
    * If NULL is specified, it should activate the back buffer.
    *
    * @param renderTarget
    * @param targetIdx
    */
   virtual void activateRenderTarget( RenderTarget* renderTarget, uint targetIdx ) = 0;

   /**
    * This method should render inactive the specified render target slot on the underlying rendering device.
    *
    * @param targetIdx
    */
   virtual void deactivateRenderTarget( uint targetIdx ) = 0;

   /**
    * This method should clean the currently set rendering target
    * using the specified color.
    *
    * @param bgColor
    * @param targetIdx
    */
   virtual void cleanRenderTarget( const Color& bgColor ) = 0;

private:
   bool shouldRenderTargetBeCleaned( RenderTarget* renderTarget ) const;
   void markRenderTargetCleaned( RenderTarget* renderTarget );
   void resetRenderTargetsList();

   /**
    * Executes the render commands.
    */
   void executeCommands();

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

#include "core-Renderer\Renderer.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _RENDERER_H
