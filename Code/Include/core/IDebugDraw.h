/// @file   core/IDebugDraw.h
/// @brief  a common debug draw interface of the engine
#pragma once

#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Renderer;

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
    * Should be called when it's time to render some stuff using a debug renderer.
    *
    * @param renderer
    */
   virtual void onDebugRender( Renderer& renderer ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A composite capable of holding many debug drawables and exposing them 
 * to the host mechanism as a single instance.
 */
class DebugScene : public IDebugDrawable
{
private:
   std::vector< IDebugDrawable* >   m_drawables;

public:
   /**
    * Adds a new drawable to the scene.
    *
    * @param drawable
    */
   void add( IDebugDrawable& drawable );

   /**
    * Removes a drawable from the scene.
    *
    * @param drawable
    */
   void remove( IDebugDrawable& drawable );

   /**
    * Clears the contents of the debug scene.
    */
   void clear();

   // -------------------------------------------------------------------------
   // IDebugDrawable implementation
   // -------------------------------------------------------------------------
   void onDebugRender( Renderer& renderer ) const;
};

///////////////////////////////////////////////////////////////////////////////
