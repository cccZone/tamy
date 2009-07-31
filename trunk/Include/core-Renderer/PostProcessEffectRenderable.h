#pragma once

#include "core-Renderer\Renderable.h"
#include "core\Observer.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Renderer;
enum RendererOps;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a very special graphical node. It is supposed to contain 
 * geometry that creates a rectangle covering the entire screen.
 *
 * It should also:
 *   - be rendered camera independently, always covering
 *     the entire area of the screen
 *   - not be influenced by the light
 *   - clamp the textures displayed on it
 *   - be rendered irrespective of the z-buffer contents (ensuring it's always rendered)
 */
class PostProcessEffectRenderable : public Renderable,
                              public Observer<Renderer, RendererOps>
{
private:
   Renderer& m_renderer;

public:
   PostProcessEffectRenderable(Renderer& renderer);

   virtual ~PostProcessEffectRenderable();

   /**
    * @derrived
    */
   virtual void render() = 0;

   /**
    * Since the object needs to always cover the entire screen,
    * we need to make it aware of the screen geometry changes.
    * That's why we'll be observing the render to react to such changes
    */
   void update(Renderer& renderer);
   void update(Renderer& renderer, const RendererOps& operation);

protected:
   virtual void recreateGeometry(unsigned int width, unsigned int height) = 0;
};


///////////////////////////////////////////////////////////////////////////////
