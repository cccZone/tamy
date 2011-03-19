/// @file   core/IDebugDraw.h
/// @brief  a common debug draw interface of the engine
#pragma once

#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct Color;

///////////////////////////////////////////////////////////////////////////////

/**
 * A common debug draw interface the engine entities can use to output
 * debug information onto the screen.
 * The reason the interface is kept here and not in the renderer library
 * is that we want all the projects of the engine to have access to it,
 * however we don't want to make them dependent on the renderer - because
 * we might simply not be using the same renderer implementation on all occasions.
 */
class IDebugDraw
{
public:
   virtual ~IDebugDraw() {}

   /**
    * Adds a line to drawing.
    *
    * @param start
    * @param end
    * @param color
    */
   virtual void drawLine( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color ) = 0;

   /**
    * Draws an arced line.
    *
    * @param start
    * @param end
    * @param color
    */
   virtual void drawArc( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color ) = 0;

   /**
    * Adds a line with an arrow to drawing.
    *
    * @param start
    * @param end
    * @param color
    */
   virtual void drawArrow( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color ) = 0;

   /**
    * Adds a box to drawing. The origin is located in the box's center.
    *
    * @param transform
    * @param size
    * @param color
    */
   virtual void drawBox( const D3DXMATRIX& transform, const D3DXVECTOR3& size, const Color& color ) = 0;

   /**
    * Adds a sphere to drawing.
    *
    * @param transform
    * @param radius
    * @param color
    */
   virtual void drawSphere( const D3DXMATRIX& transform, float radius, const Color& color ) = 0;

   /**
    * Adds a cylinder to drawing. The origin is located in the cylinder's bottom face's center.
    *
    * @param transform
    * @param radius
    * @param height
    * @param color
    */
   virtual void drawCylinder( const D3DXMATRIX& transform, float radius, float height, const Color& color ) = 0;
};

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
    * @param renderer      debug renderer
    */
   virtual void onDebugRender( IDebugDraw& renderer ) const = 0;
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

   // -------------------------------------------------------------------------
   // IDebugDrawable implementation
   // -------------------------------------------------------------------------
   void onDebugRender( IDebugDraw& renderer ) const;
};

///////////////////////////////////////////////////////////////////////////////
