/// @file   TamyEditor/MBInstanceTexture.h
/// @brief  material pipeline block representing an instance texture provider node
#pragma once

#include "core-Renderer/MNInstanceTexture.h"
#include "GraphBlock.h"
#include "core/ResourceHandle.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * Material pipeline block representing an instance texture provider node.
 */
class MBInstanceTexture : public TGraphBlock< MNInstanceTexture, MaterialNode >
{
   DECLARE_CLASS()

public:
   /**
    * Default constructor required by the RTTI system.
    */
   MBInstanceTexture() : TGraphBlock< MNInstanceTexture, MaterialNode >() {}

   /**
    * Parametrized constructor required by the generic factory.
    *
    * @param node    represented node
    */
   MBInstanceTexture( MNInstanceTexture& node );
};

///////////////////////////////////////////////////////////////////////////////
