/// @file   core-Renderer/Renderable.h
/// @brief  an item that can be rendered by the rendering view
#pragma once

#include "core-Renderer\EffectAttribute.h"


///////////////////////////////////////////////////////////////////////////////

class Renderable
{
public:
   virtual ~Renderable() {}

   /**
    * Renders the renderable.
    */
   virtual void render() = 0;

   /**
    * Returns the list of all attributes of the renderable.
    * Attributes are taken from the effects the renderable 
    * contains.
    *
    * @return  effects attributes collection
    */
   virtual const Attributes& getAttributes() const = 0;
};

///////////////////////////////////////////////////////////////////////////////
