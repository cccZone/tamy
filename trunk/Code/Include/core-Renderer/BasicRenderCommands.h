/// @file   core-Renderer/BasicRenderCommands.h
/// @brief  basic rendering commands marshaled to the underlying graphics library
#pragma once

#include "core-Renderer/RenderCommand.h"
#include "core/Color.h"
#include "core/types.h"
#include "core/Matrix.h"


///////////////////////////////////////////////////////////////////////////////

enum MaterialColor
{
   MC_Ambient,
   MC_Diffuse,
   MC_Specular,
   MC_Emissive
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command sets the selected material component used by the fixed rendering pipeline.
 */
class RCSetFixedMaterial  : public RenderCommand
{
private:
   MaterialColor  m_materialComponent;
   Color          m_color;

public:
   /**
    * Constructor.
    *
    * @param materialComponent
    * @param color
    */
   RCSetFixedMaterial( MaterialColor materialComponent, const Color& color ) : m_materialComponent( materialComponent ), m_color( color ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command sets the global ambient color.
 */
class RCSetAmbientColor  : public RenderCommand
{
private:
   Color       m_color;

public:
   /**
    * Constructor.
    *
    * @param color
    */
   RCSetAmbientColor( const Color& color ) : m_color( color ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command defines the use of Z-buffer.
 */
class RCSetZBuffer  : public RenderCommand
{
private:
   bool           m_performTest;
   bool           m_enableWrite;

public:
   /**
    * Constructor.
    *
    * @param performTest
    * @param enableWrite
    */
   RCSetZBuffer( bool performTest, bool enableWrite ) : m_performTest( performTest ), m_enableWrite( enableWrite ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////


/**
 * Command defines whether the fixed lightning pipeline should be enabled.
 */
class RCEnableLighting  : public RenderCommand
{
private:
   bool           m_enable;

public:
   /**
    * Constructor.
    *
    * @param enable
    */
   RCEnableLighting( bool enable ) : m_enable( enable ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

enum GeometryFillMode
{
   GFM_Point               = 1,
   GFM_Wireframe           = 2,
   GFM_Solid               = 3,
   GFM_ForceByte           = 0x7f, // force 8-bit size enum
};

/**
 * Command defines the way in which the rendered geometry should be drawn.
 */
class RCSetFillMode  : public RenderCommand
{
private:
   GeometryFillMode       m_fillMode;

public:
   /**
    * Constructor.
    *
    * @param fillMode
    */
   RCSetFillMode( GeometryFillMode fillMode ) : m_fillMode( fillMode ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

enum SourceVertexColor
{
   SVC_Material           = 0,            // Color from material is used
   SVC_Diffuse            = 1,            // Diffuse vertex color is used
   SVC_Specular           = 2,            // Specular vertex color is used
   SVC_ForceByte          = 0x7f          // force 8-bit size enum
};

/**
 * Command defines the color source for the particular material component color.
 */
class RCSetColorSource  : public RenderCommand
{
private:
   MaterialColor              m_materialColor;
   SourceVertexColor          m_sourceVertexColor;

public:
   /**
    * Constructor.
    *
    * @param materialColor
    * @param sourceVertexColor
    */
   RCSetColorSource( MaterialColor materialColor, SourceVertexColor sourceVertexColor ) : m_materialColor( materialColor ), m_sourceVertexColor( sourceVertexColor ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command defines the description of the vertex structure used by the rendered geometry.
 */
class RCSetVertexDeclaration  : public RenderCommand
{
private:
   ulong                      m_vertexDeclaration;

public:
   /**
    * Constructor.
    *
    * @param vertexDeclaration
    */
   RCSetVertexDeclaration( ulong vertexDeclaration ) : m_vertexDeclaration( vertexDeclaration ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command sets the world matrix.
 */
class RCSetWorldMatrix  : public RenderCommand
{
private:
   Matrix                      m_mtx;

public:
   /**
    * Constructor.
    *
    * @param mtx
    */
   RCSetWorldMatrix( const Matrix& mtx ) : m_mtx( mtx ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Command sets the camera matrices.
 */
class RCSetCameraMatrices  : public RenderCommand
{
private:
   Matrix                      m_viewMtx;
   Matrix                      m_projectionMtx;

public:
   /**
    * Constructor.
    *
    * @param viewMtx
    * @param projectionMtx
    */
   RCSetCameraMatrices( const Matrix& viewMtx, const Matrix& projectionMtx ) : m_viewMtx( viewMtx ), m_projectionMtx( projectionMtx ) {}

   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that begins the process of rendering a scene.
 */
class RCBeginScene : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that ends the process of rendering a scene.
 */
class RCEndScene : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Render command that clears the contents of a depth buffer.
 */
class RCClearDepthBuffer : public RenderCommand
{
public:
   // -------------------------------------------------------------------------
   // RenderCommand implementation
   // -------------------------------------------------------------------------
   void execute( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
