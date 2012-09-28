/// @file   TamyEditor/GBSpatialEntity.h
/// @brief  geometry shader pipeline block representing a node that provides info about parent spatial entity
#pragma once

#include "core-Renderer/GNSpatialEntity.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

class GBSpatialEntity : public TGraphBlock< GNSpatialEntity, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBSpatialEntity() : TGraphBlock< GNSpatialEntity, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBSpatialEntity( GNSpatialEntity& node );
};

///////////////////////////////////////////////////////////////////////////////
