/// @file   core-Renderer/MNSpatialEntity.h
/// @brief  a node that provides information about the parent node the material renders
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSMatrixOutput;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

class MNSpatialEntity : public MaterialNode
{
   DECLARE_CLASS( MNSpatialEntity )

private:
   MSMatrixOutput*                     m_worldMtx;
   MSMatrixOutput*                     m_worldViewMtx;

   TRuntimeVar< SpatialEntity* >       m_parentNode;

public:
   /**
    * Constructor.
    */
   MNSpatialEntity();
   ~MNSpatialEntity();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void onCreateLayout( const MaterialEntity& host ) const;
   void preRender( Renderer& renderer, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
