/// @file   core-Renderer/RPVec4Node.h
/// @brief  a node that provides a vector value
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"
#include "core/Vector.h"


///////////////////////////////////////////////////////////////////////////////

class RPVec4Output;

///////////////////////////////////////////////////////////////////////////////

class RPVec4Node : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   Vector            m_vector;

   // runtime data
   RPVec4Output*    m_output;
 
public:
   /**
    * Constructor.
    */
   RPVec4Node();
   ~RPVec4Node();

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
