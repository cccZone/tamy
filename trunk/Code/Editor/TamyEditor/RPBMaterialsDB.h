/// @file   TamyEditor/RPBMaterialsDB.h
/// @brief  rendering pipeline block that manages the materials DB
#pragma once

#include "core-Renderer/RPMaterialsDBNode.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Rendering pipeline block that manages the materials DB.
 */
class RPBMaterialsDB : public TGraphBlock< RPMaterialsDBNode, RenderingPipelineNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   RPBMaterialsDB() : TGraphBlock< RPMaterialsDBNode, RenderingPipelineNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   RPBMaterialsDB( RPMaterialsDBNode& node );
};

///////////////////////////////////////////////////////////////////////////////
