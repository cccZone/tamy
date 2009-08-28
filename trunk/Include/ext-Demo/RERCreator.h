#pragma once

/// @file   ext-Demo\RERCreator.h
/// @brief  programmable effects renderers instantiator

///////////////////////////////////////////////////////////////////////////////

class RenderingEffectRenderer;
class GraphicalEffect;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This class instantiates a rendering controller that operates on an effect.
 */
class RERCreator
{
public:
   virtual ~RERCreator() {}

   /**
    * Instantiate a rendering controller that will operate 
    * on the specified effect.
    *
    * @param effect     effect that will be used as a renderer
    */
   virtual RenderingEffectRenderer* create(GraphicalEffect* effect) = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class creates implementations of RenderingEffectRenderer
 * it was templated with.
 */
template<typename T>
class TRERCreator : public RERCreator
{
public:
   RenderingEffectRenderer* create(GraphicalEffect* effect)
   {
      return new T(effect);
   }
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo
