/// @file   TamyEditor/MBTexture.h
/// @brief  material pipeline block representing a texture provider node
#pragma once

#include "core-Renderer/MNTexture.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a texture provider node.
 */
class MBTexture : public TPipelineBlock< MNTexture, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBTexture() : TPipelineBlock< MNTexture, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBTexture( MNTexture& node );
};

///////////////////////////////////////////////////////////////////////////////
