#pragma once

#include <d3dx9.h>
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Material;

///////////////////////////////////////////////////////////////////////////////

/**
 * An atomic object that can be rendered. It can consist only of one
 * material and has a transformation.
 *
 * A complex object (one that has many materials) can consist of
 * many renderables. The way they are organised depends on the object
 * representation - however in order to render it, we need to pass
 * all the objects to the RenderingMechanism in a single Array.
 */
class Renderable
{
public:
   virtual ~Renderable() {}

   /** 
    * Renders the renderable.
    */
   virtual void render() = 0;

   /** 
    * This method returns an array of matrices that should be
    * used to position the renderable during rendering.
    *
    * @return        array of D3DXMATRIX sturctures
    */
   virtual const Array<D3DXMATRIX>& getRenderingMatrices() = 0;
   
   /**
    * Returns a material that should be used to render the node.
    *
    * @return        material that should be used during rendering
    */
   virtual const Material& getMaterial() const = 0;

   /**
    * Returns the transformation of a renderable in world space.
    *
    * @return        object world transformation
    */
   virtual const D3DXMATRIX& getGlobalTransform() = 0;
};

///////////////////////////////////////////////////////////////////////////////
