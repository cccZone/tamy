/// @file   TamyEditor/MBMaterialIndex.h
/// @brief  material pipeline block representing a material index provider node
#pragma once

#include "core-Renderer/MNMaterialIndex.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a material index provider node.
 */
class MBMaterialIndex : public TPipelineBlock< MNMaterialIndex, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBMaterialIndex() : TPipelineBlock< MNMaterialIndex, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBMaterialIndex( MNMaterialIndex& node );
};

///////////////////////////////////////////////////////////////////////////////
