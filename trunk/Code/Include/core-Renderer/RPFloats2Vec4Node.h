/// @file   core-Renderer/RPFloats2Vec4Node.h
/// @brief  a node that packs 4 floats to a single vector
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPFloatInput;
class RPVec4Output;

///////////////////////////////////////////////////////////////////////////////

class RPFloats2Vec4Node : public RenderingPipelineNode
{
   DECLARE_CLASS( RPFloats2Vec4Node )

private:
   RPFloatInput*     m_dataInput[4];
   RPVec4Output*     m_output;

public:
   /**
    * Constructor.
    */
   RPFloats2Vec4Node();
   ~RPFloats2Vec4Node();

   // -------------------------------------------------------------------------
   // RenderingPipelineNode implementation
   // -------------------------------------------------------------------------
   void onUpdate( RenderingPipelineMechanism& host ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
};

///////////////////////////////////////////////////////////////////////////////
