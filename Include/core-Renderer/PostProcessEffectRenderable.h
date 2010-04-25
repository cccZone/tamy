#pragma once

/// @file   core-Renderer\PostProcessEffectRenderable.h
/// @brief  a post process effect renderable object


#include "core\Observer.h"
#include "core-Renderer\Renderable.h"
#include "core\BoundingSpace.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Renderer;
enum RendererOps;
class Texture;
class GraphicalEntitiesFactory;

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a very special renderable. It is supposed to contain 
 * geometry that creates a rectangle covering the entire screen.
 *
 * It should also:
 *   - be rendered camera independently, always covering
 *     the entire area of the screen
 *   - not be influenced by the light
 *   - clamp the textures displayed on it
 *   - be rendered irrespective of the z-buffer contents (ensuring it's always rendered)
 */
class PostProcessEffectRenderable : public Observer<Renderer, RendererOps>,
                                    public Renderable
{
private:
   Renderer& m_renderer;
   Array<D3DXMATRIX> m_renderingMatrices;
   D3DXMATRIX m_worldTransform;
   Material* m_defaultMat;
   BoundingSpace m_volume;

public:
   PostProcessEffectRenderable(Renderer& renderer);

   virtual ~PostProcessEffectRenderable();

   /**
    * @inherited
    */
   virtual void render() = 0;

   const Array<D3DXMATRIX>& getRenderingMatrices();
   
   const Material& getMaterial() const;

   const BoundingVolume& getVolume();

   /**
    * The looks of this renderable is defined by the textures 
    * that are set on its material.
    * To facilitate the process of changing this look, this method was added.
    * It allows to easily add a number of textures, which the post process
    * effect will be operating on.
    *
    * @param texture    texture for an effect to process
    * @param factory    graphical entities factory needed by the method
    *                   to create proper strucutres that will be needed
    *                   to set the texture up correctly
    */
   void addInputTexture(Texture& texture, GraphicalEntitiesFactory& factory);

   const D3DXMATRIX& getGlobalTransform();

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
