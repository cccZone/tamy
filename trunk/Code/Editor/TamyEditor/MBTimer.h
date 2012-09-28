/// @file   TamyEditor/MBTimer.h
/// @brief  material pipeline block representing a time provider node
#pragma once

#include "core-Renderer/MNTimer.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing a time provider node.
 */
class MBTimer : public TGraphBlock< MNTimer, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBTimer() : TGraphBlock< MNTimer, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBTimer( MNTimer& node );
};

///////////////////////////////////////////////////////////////////////////////
