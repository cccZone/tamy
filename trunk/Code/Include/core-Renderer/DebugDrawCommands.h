/// @file   core-Renderer/DebugDrawCommands.h
/// @brief  commands for rendering debug stuff
#pragma once

#include "core-Renderer/RenderCommand.h"
#include "core/Color.h"
#include "core/Matrix.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

#define OVERLAY      true

///////////////////////////////////////////////////////////////////////////////

class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing a debug line.
 */
class RCDrawDebugLine : public RenderCommand
{
private:
   Vector            m_start;
   Vector            m_end;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param start
    * @param end
    * @param color
    */
   RCDrawDebugLine( const Vector& start, const Vector& end, const Color& color )
      : m_start( start )
      , m_end( end )
      , m_color( color )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing an arced debug line.
 */
class RCDrawDebugArc : public RenderCommand
{
private:
   Vector            m_start;
   Vector            m_end;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param start
    * @param end
    * @param color
    */
   RCDrawDebugArc( const Vector& start, const Vector& end, const Color& color )
      : m_start( start )
      , m_end( end )
      , m_color( color )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing a debug line with arrow head.
 */
class RCDrawDebugArrow : public RenderCommand
{
private:
   float             m_lineWidth;
   Vector            m_start;
   Vector            m_end;
   Color             m_color;
   bool              m_overlay;

public:
   /**
    * Constructor.
    *
    * @param lineWidth
    * @param start
    * @param end
    * @param color
    * @param overlay
    */
   RCDrawDebugArrow( float lineWidth, const Vector& start, const Vector& end, const Color& color, bool overlay = false )
      : m_lineWidth( lineWidth )
      , m_start( start )
      , m_end( end )
      , m_color( color )
      , m_overlay( overlay )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing a debug box.
 */
class RCDrawDebugBox : public RenderCommand
{
private:
   Matrix            m_transform;
   Vector            m_size;
   Color             m_color;
   bool              m_overlay;

public:
   /**
    * Constructor.
    *
    * @param transform
    * @param size
    * @param color
    * @param overlay
    */
   RCDrawDebugBox( const Matrix& transform, const Vector& size, const Color& color, bool overlay = false )
      : m_transform( transform )
      , m_size( size )
      , m_color( color )
      , m_overlay( overlay )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing a debug ring.
 */
class RCDrawDebugRing : public RenderCommand
{
private:
   float             m_circumferenceWidth;
   float             m_radius;
   Matrix            m_transform;
   Color             m_color;
   bool              m_overlay;

public:
   /**
    * Constructor.
    *
    * @param circumferenceWidth
    * @param transform
    * @param radius
    * @param color
    * @param overlay
    */
   RCDrawDebugRing( float circumferenceWidth, const Matrix& transform, float radius, const Color& color, bool overlay = false )
      : m_circumferenceWidth( circumferenceWidth )
      , m_radius( radius )
      , m_overlay( overlay )
      , m_transform( transform )
      , m_color( color )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing a debug sphere.
 */
class RCDrawDebugSphere : public RenderCommand
{
private:
   Matrix            m_transform;
   float             m_radius;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param transform
    * @param radius
    * @param color
    */
   RCDrawDebugSphere( const Matrix& transform, float radius, const Color& color )
      : m_transform( transform )
      , m_radius( radius )
      , m_color( color )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////


/**
 * Rendering command for drawing a debug cylinder. The origin is located 
 * in the cylinder's bottom face's center.
 */
class RCDrawDebugCylinder : public RenderCommand
{
private:
   Matrix            m_transform;
   float             m_radius;
   float             m_height;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param transform
    * @param radius
    * @param height
    * @param color
    */
   RCDrawDebugCylinder( const Matrix& transform, float radius, float height, const Color& color )
      : m_transform( transform )
      , m_radius( radius )
      , m_height( height )
      , m_color( color )
   {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Draws a debug grid.
 */
class RCDrawDebugGrid : public RenderCommand
{
private:
   float       m_gridSize;
   float       m_gridLinesSpacing;
   Color       m_gridColor;

public:
   /**
    * Constructor.
    */
   RCDrawDebugGrid( float gridSize, float gridLinesSpacing, const Color& gridColor )
      : m_gridSize( gridSize )
      , m_gridLinesSpacing( gridLinesSpacing )
      , m_gridColor( gridColor )
   {
   }

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command that begins the rendering process of a debug scene.
 */
class RCBeginDebugScene : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command that ends the rendering process of a debug scene.
 */
class RCEndDebugScene : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
