/// @file   TamyEditor/MBFloat.h
/// @brief  material pipeline block representing a float value provider node
#pragma once

#include "core-Renderer/MNFloat.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a float value provider node.
 */
class MBFloat : public TGraphBlock< MNFloat, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBFloat() : TGraphBlock< MNFloat, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBFloat( MNFloat& node );
};

///////////////////////////////////////////////////////////////////////////////
