#pragma once

#include "core\Array.h"
#include "core-Renderer\RenderingMechanism.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class RenderingAction;
class RenderingTargetsPolicy;
class RendererImpl;
class RenderablesStorage;
class Camera;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class takes care of translating whatever's inside the graphical
 * nodes into a list of rendering commands that are executed
 * by an arbitrary rendering abstraction layer.
 */
class RenderingMechanismLeaf : public RenderingMechanism
{
private:
   RenderingTargetsPolicy* m_policy;
   RenderablesStorage* m_nodesStorage;
   RenderingAction* m_renderingAction;
   Camera* m_activeCamera;
   RendererImpl* m_impl;

   Array<Renderable*> m_objectsToRender;

public:
   /** 
    * Constructor.
    *
    * @param policy     policy for rendering targets this mechanism should use
    * @param impl       implementation of a renderer that will carry out
    *                   actual rendering commands
    * @param cleaner    rendering targets cleaner
    */
   RenderingMechanismLeaf(RenderingTargetsPolicy* policy, 
                          RendererImpl* impl);
   ~RenderingMechanismLeaf();

   /**
    * The method renders renderables supplied by the storage.
    */
   void render();

   /** 
    * Allows to set a node storage which the mechanism will query
    * for nodes to render.
    *
    * @param storage    storage containing renderable nodes
    */
   void setStorage(RenderablesStorage* storage);

   /**
    * Sets a new rendering action.
    *
    * @param action     new action that will be performed
    *                   when a single node is rendered
    */
   void setRenderingAction(RenderingAction* action);

   /**
    * Sets an instance of camera that should be used for rendering.
    *
    * @param camera     camera we want to use for rendering
    */
   void setActiveCamera(Camera& camera);
};

///////////////////////////////////////////////////////////////////////////////
