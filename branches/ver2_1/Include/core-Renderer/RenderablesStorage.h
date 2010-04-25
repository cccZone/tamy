#pragma once

/// @file   core-Renderer\RenderablesStorage.h
/// @brief  container that stores Renderables for 
///         a RenderingMechanism to render

#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class Camera;
class Renderable;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface provides na instance of RenderingMechanism
 * with access to renderable nodes.
 */
class RenderablesStorage
{
public:
   virtual ~RenderablesStorage() {}

   /** 
    * The method returns an array of renderables visible from the specified
    * camera position.
    *
    * @param camera           camera that we're using to render renderables
    *
    * @param outRenderables   the method will fill this array with renderables
    *                         it finds
    */
   virtual void query(Camera& camera, Array<Renderable*>& outNodes) = 0;
};

///////////////////////////////////////////////////////////////////////////////
