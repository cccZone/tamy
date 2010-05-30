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
    * Sets an attributes sorter.
    *
    * @param sorter
    */
   void setAttributeSorter( AttributeSorter& sorter );

   // ----------------------------------------------------------------------
   // ModelView implementation
   // ----------------------------------------------------------------------
   void onEntityAdded(Entity& entity);
   void onEntityRemoved(Entity& entity);
   void onEntityChanged(Entity& entity);

protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////
