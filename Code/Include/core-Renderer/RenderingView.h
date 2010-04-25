#pragma once

/// @file   core-Renderer\RenderingView.h
/// @brief  view that renders renderable entities

#include <map>
#include "core-MVC\ModelView.h"


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class RenderableRepresentation;
class AttributeSorter;

///////////////////////////////////////////////////////////////////////////////

/**
* This view manages the visibility of renderables.
*/ 
class RenderingView : public ModelView
{
private:
   typedef std::map<Renderable*, RenderableRepresentation*> RenderablesMap;
   RenderablesMap m_renderables;

   AttributeSorter* m_sorter;

public:
   /**
    * Constructor.
    */
   RenderingView();
   ~RenderingView();

   /**
    * Changes the way the renderables will be batched for rendering
    * by using a different attribute sorting strategy.
    *
    * @param sorter  new effect attributes sorter
    */
   void setAttributeSorter(AttributeSorter* sorter);

   /**
    * Updates the view (by rendering the renderables it contains).
    */
   void update();

   // ----------------------------------------------------------------------
   // ModelView implementation
   // ----------------------------------------------------------------------
   void onEntityAdded(Entity& entity);

   void onEntityRemoved(Entity& entity);

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
