/// @file   core-Renderer/DebugDrawCommands.h
/// @brief  commands for rendering debug stuff
#pragma once

#include "core-Renderer/RenderCommand.h"
#include "core/Color.h"
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct Color;
class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering command for drawing a debug line.
 */
class RCDrawLine : public RenderCommand
{
private:
   D3DXVECTOR3       m_start;
   D3DXVECTOR3       m_end;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param start
    * @param end
    * @param color
    */
   RCDrawLine( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color )
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
class RCDrawArc : public RenderCommand
{
private:
   D3DXVECTOR3       m_start;
   D3DXVECTOR3       m_end;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param start
    * @param end
    * @param color
    */
   RCDrawArc( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color )
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
class RCDrawArrow : public RenderCommand
{
private:
   D3DXVECTOR3       m_start;
   D3DXVECTOR3       m_end;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param start
    * @param end
    * @param color
    */
   RCDrawArrow( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color )
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
 * Rendering command for drawing a debug box.
 */
class RCDrawBox : public RenderCommand
{
private:
   D3DXMATRIX        m_transform;
   D3DXVECTOR3       m_size;
   Color             m_color;

public:
   /**
    * Constructor.
    *
    * @param transform
    * @param size
    * @param color
    */
   RCDrawBox( const D3DXMATRIX& transform, const D3DXVECTOR3& size, const Color& color )
      : m_transform( transform )
      , m_size( size )
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
class RCDrawSphere : public RenderCommand
{
private:
   D3DXMATRIX        m_transform;
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
   RCDrawSphere( const D3DXMATRIX& transform, float radius, const Color& color )
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
class RCDrawCylinder : public RenderCommand
{
private:
   D3DXMATRIX        m_transform;
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
   RCDrawCylinder( const D3DXMATRIX& transform, float radius, float height, const Color& color )
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
