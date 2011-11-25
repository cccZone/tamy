/// @file   core-Renderer/MNSpatialEntity.h
/// @brief  a node that provides information about the parent node the material renders
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSMatrixOutput;

///////////////////////////////////////////////////////////////////////////////

class MNSpatialEntity : public MaterialNode
{
   DECLARE_CLASS( MNCamera )

private:
   MSMatrixOutput*   m_worldMtx;
   MSMatrixOutput*   m_worldViewMtx;

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
};

///////////////////////////////////////////////////////////////////////////////
