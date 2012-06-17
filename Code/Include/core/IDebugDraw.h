/// @file   core/IDebugDraw.h
/// @brief  a common debug draw interface of the engine
#pragma once


///////////////////////////////////////////////////////////////////////////////

class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * An interface that will allow an instance of a class to draw its contents
 * on a debug renderer.
 *
 * The mechanism binding the two interfaces is not implemented here - because
 * there may be arbitrary implementations of such a mechanism. 
 * What we want here are two marker interface that will unify classes
 * across the engine to use the same debug rendering API.
 */
class IDebugDrawable
{
public:
   virtual ~IDebugDrawable() {}

   /**
    * Initialize the debug geometry in this method
    *
    * @param renderer
    */
   virtual void onInitializeDebugRepresentation( Model& debugScene ) const = 0;

   /**
    * Deinitialize the debug geometry in this method.
    */
   virtual void onDeinitializeDebugRepresentation( Model& debugScene ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

