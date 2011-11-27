/// @file   core-Renderer/GNCamera.h
/// @brief  a node that provides information about the active camera
#pragma once

#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

class GSFloatOutput;
class GSMatrixOutput;

///////////////////////////////////////////////////////////////////////////////

class GNCamera : public GeometryShaderNode
{
   DECLARE_CLASS( GNCamera )

private:
   GSFloatOutput*    m_nearZ;
   GSFloatOutput*    m_farZ;
   GSFloatOutput*    m_viewportWidth;
   GSFloatOutput*    m_viewportHeight;
   GSMatrixOutput*   m_view;
   GSMatrixOutput*   m_proj;
   GSMatrixOutput*   m_worldView;
   GSMatrixOutput*   m_worldViewProj;

public:
   /**
    * Constructor.
    */
   GNCamera();
   ~GNCamera();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // GeometryShaderNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const GeometryEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
