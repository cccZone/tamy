/// @file   TamyEditor/MBVec4.h
/// @brief  material pipeline block representing a vector provider node
#pragma once

#include "core-Renderer/MNVec4.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a vector provider node.
 */
class MBVec4 : public TGraphBlock< MNVec4, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBVec4() : TGraphBlock< MNVec4, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBVec4( MNVec4& node );
};

///////////////////////////////////////////////////////////////////////////////
