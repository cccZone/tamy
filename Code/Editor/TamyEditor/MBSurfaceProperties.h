/// @file   TamyEditor/MBSurfaceProperties.h
/// @brief  material pipeline block representing an instance surface properties provider
#pragma once

#include "core-Renderer/MNSurfaceProperties.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing an instance surface properties provider.
 */
class MBSurfaceProperties : public TGraphBlock< MNSurfaceProperties, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBSurfaceProperties() : TGraphBlock< MNSurfaceProperties, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBSurfaceProperties( MNSurfaceProperties& node );
};

///////////////////////////////////////////////////////////////////////////////
