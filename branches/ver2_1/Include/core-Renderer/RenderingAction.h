#pragma once


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class RendererImpl;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering action is executed when a single Renderable is rendered.
 * It can be anything - from setting a material, setting up lights, doing
 * some additional calculations - the point is that it's done on a PER-NODE
 * basis (per-visible object basis to be exact) and it's aim is to help
 * get the object rendered
 */
class RenderingAction
{
public:
   virtual ~RenderingAction() {}

   /**
    * This method is called just before the node's geometry
    * is rendered
    *
    * @param renedrable    object that in a sec will be subject to rendering
    * @param impl          implementation of a renderer capable of performing
    *                      the actual rendering operations (bridge pattern)
    */
   virtual void onRenderingBegin(Renderable& renderable, 
                                 RendererImpl& impl) = 0;

   /**
    * This method is called just after the node's geometry
    * was rendered
    *
    * @param renderable    object that has just been rendered
    * @param impl          implementation of a renderer capable of performing
    *                      the actual rendering operations (bridge pattern)
    */
   virtual void onRenderingEnd(Renderable& renderable, 
                               RendererImpl& impl) = 0;
};

///////////////////////////////////////////////////////////////////////////////
