/// @file   TamyEditor/GBCamera.h
/// @brief  geometry shader pipeline block representing a camera node
#pragma once

#include "core-Renderer/GNCamera.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Geometry shader pipeline block representing a camera node.
 */
class GBCamera : public TGraphBlock< GNCamera, GeometryShaderNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   GBCamera() : TGraphBlock< GNCamera, GeometryShaderNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   GBCamera( GNCamera& node );
};

///////////////////////////////////////////////////////////////////////////////
