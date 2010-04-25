#pragma once

#include <d3dx9.h>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class LightReflectingProperties;
class Texture;
class MaterialOperation;
class CoordinatesOperation;
class Renderable;
class RenderingTargetsPolicy;
class Light;
class Background;

///////////////////////////////////////////////////////////////////////////////

/**
 * This rendering abstraction layer is responsible for 
 * realisation of atomic rendering commands on the underlying graphical 
 * library.
 */
class RendererImpl
{
public:
   virtual ~RendererImpl() {}

   /**
    * The method renders the contents of a node using the specified rendering
    * targets policy.
    *
    * @param renderable    renderable which we want rendered
    * @param policy        rendering targets policy that should be used during 
    *                      rendering
    */
   virtual void render(Renderable& renderable, RenderingTargetsPolicy& policy) = 0;

   /**
    * The method enables specified lights.
    *
    * @param lights        array containing the lights we want enabled
    */
   virtual void enableLights(const Array<Light*>& lights) = 0;

   /**
    * The method disables specified lights.
    *
    * @param lights        array containing the lights we want disabled
    */
   virtual void disableLights(const Array<Light*>& lights) = 0;

   /**
    * The method sets the view and projection matrices
    *
    * @param viewMtx    view matrix
    * @param projMtx    projection matrix
    */
   virtual void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx) = 0;

   /**
    * The method sets rendered object's world matrices. 
    * If there are more than one, it means that it's a skin that is rendered.
    *
    * @param matrices   object's world matrices
    */
   virtual void setObjectMatrices(const Array<D3DXMATRIX>& matrices) = 0;

   /**
    * The method informs whether the object we're rendering transparent
    * and sets up the underlying pipeline in a way that enables to draw
    * objects transparent.
    *
    * @param enable     'true' means that we wish to render transparent 
    *                   object
    */
   virtual void setTransparency(bool enable) = 0;
   
   /** 
    * Sets the light reflecting properties of the rendered object.
    *
    * @param lrp        light reflecting properties
    */
   virtual void setLRP(LightReflectingProperties& lrp) = 0;

   /**
    * Sets a texture that should be put on the rendered object.
    * Passing the stage index opens way to set more than one texture
    * on the device, thus allowing for multitexturing.
    *
    * @param stageIndex    sampler stage the texture should be 
    *                      assigned to
    * @param texture       texture we want to use during rendering
    */
   virtual void setTexture(unsigned char stageIndex, Texture& texture) = 0;

   /**
    * Textures from multiple stages can be blended together to create
    * a more sophisticated looking texture - this method specifies
    * an operation that should be used to blend the color values specified
    * sampler stage contributes.
    *
    * @param stageIndex    sampler stage index
    * @param colorOp       operation on color channels
    */
   virtual void setColorOperation(unsigned char stageIndex, 
                                  MaterialOperation& colorOp) = 0;

   /**
    * Textures from multiple stages can be blended together to create
    * a more sophisticated looking texture - this method specifies
    * an operation that should be used to blend the alpha channel values
    * specified sampler stage contributes.
    *
    * @param stageIndex    sampler stage index
    * @param alphaOp       operation on alpha channels
    */
   virtual void setAlphaOperation(unsigned char stageIndex, 
                                  MaterialOperation& alphaOp) = 0;

   /**
    * Specifies the strategy for calculating texture coordinates
    * when those exceed the natural range 0.0 - 1.0 (clamping, wrapping etc.)
    *
    * @param stageIndex    sampler stage index
    * @param coordsOp      operation on texture coordinates
    */
   virtual void setCoordsOperation(unsigned char stageIndex, 
                                   CoordinatesOperation& coordsOp) = 0;

};

///////////////////////////////////////////////////////////////////////////////
