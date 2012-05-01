/// @file   core-Renderer/RPVec4Node.h
/// @brief  a node that provides a vector value
#pragma once

#include "core-Renderer/RenderingPipelineNode.h"


///////////////////////////////////////////////////////////////////////////////

class RPVec4Output;

///////////////////////////////////////////////////////////////////////////////

class RPVec4Node : public RenderingPipelineNode
{
   DECLARE_CLASS()

private:
   // static data
   float             m_x;
   float             m_y;
   float             m_z;
   float             m_w;

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
