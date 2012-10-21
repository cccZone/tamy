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
   DECLARE_ALLOCATOR( MNSpatialEntity, AM_ALIGNED_16 );
   DECLARE_CLASS()

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
   void onCreateLayout( const MaterialInstance& host ) const;
   void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
