/// @file   core-Renderer/GNSpatialEntity.h
/// @brief  a node that provides information about the parent node the material renders
#pragma once

#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

class GSMatrixOutput;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

class GNSpatialEntity : public GeometryShaderNode
{
   DECLARE_CLASS()

private:
   GSMatrixOutput*                     m_worldMtx;
   GSMatrixOutput*                     m_worldViewMtx;

   TRuntimeVar< SpatialEntity* >       m_parentNode;

public:
   /**
    * Constructor.
    */
   GNSpatialEntity();
   ~GNSpatialEntity();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // GeometryShaderNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( const GeometryEntity& host ) const;
   void preRender( Renderer& renderer, const GeometryEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
