/// @file   TamyEditor/MBCamera.h
/// @brief  material pipeline block representing a camera node
#pragma once

#include "core-Renderer/MNCamera.h"
#include "PipelineBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a camera node.
 */
class MBCamera : public TPipelineBlock< MNCamera, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBCamera() : TPipelineBlock< MNCamera, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBCamera( MNCamera& node );
};

///////////////////////////////////////////////////////////////////////////////
