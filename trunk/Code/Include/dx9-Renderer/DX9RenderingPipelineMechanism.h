#pragma once

/// @file   dx9-Renderer\DX9RenderingPipelineMechanism.h
/// @brief  directX9 implementation of a rendering pipeline mechanism

#include "core-Renderer\RenderingPipelineMechanism.h"
#include <d3d9.h>


///////////////////////////////////////////////////////////////////////////////


class DX9RenderingPipelineMechanism : public RenderingPipelineMechanismImpl
{
private:
   struct DebugVertex
   {
      D3DXVECTOR3       m_vtx;
      DWORD             m_color;
   };

private:
   RenderingPipelineMechanism&         m_renderingPipelineMechanism;
   IDirect3DDevice9*                   m_d3Device;

   // some limit definitions
   const unsigned int                  DEBUG_LINES_MAX_COUNT;

   // debug draw data
   D3DXMATRIX                          m_identityMtx;
   IDirect3DVertexBuffer9*             m_linesBuffer;
   DebugVertex*                        m_pVertex;
   unsigned int                        m_linesCount;

public:
   DX9RenderingPipelineMechanism( RenderingPipelineMechanism& renderingPipelineMechanism );
   ~DX9RenderingPipelineMechanism();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize(Renderer& renderer);

   // -------------------------------------------------------------------------
   // RenderingPipelineMechanismImpl implementation
   // -------------------------------------------------------------------------
   void passBegin();
   void passEnd();
   void renderDebug( Camera& activeCamera );

   // -------------------------------------------------------------------------
   // IDebugDraw implementation
   // -------------------------------------------------------------------------
   void drawLine( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color );
   void drawArc( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color );
   void drawArrow( const D3DXVECTOR3& start, const D3DXVECTOR3& end, const Color& color );
   void drawBox( const D3DXMATRIX& transform, const D3DXVECTOR3& size, const Color& color );
   void drawSphere( const D3DXMATRIX& transform, float radius, const Color& color );
   void drawCylinder( const D3DXMATRIX& transform, float radius, float height, const Color& color );

private:
   /**
    * A helper method for adding a line vertex definition
    */
   void addLineVertex( const D3DXVECTOR3& vtx, const Color& color );
};

///////////////////////////////////////////////////////////////////////////////
