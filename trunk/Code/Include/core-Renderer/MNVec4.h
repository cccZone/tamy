/// @file   core-Renderer/MNVec4.h
/// @brief  a node that provides a vector value
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSVec4Output;

///////////////////////////////////////////////////////////////////////////////

class MNVec4 : public MaterialNode
{
   DECLARE_CLASS( MNVec4 )

private:
   // static data
   float             m_x;
   float             m_y;
   float             m_z;
   float             m_w;

   // runtime data
   MSVec4Output*    m_output;
 
public:
   /**
    * Constructor.
    */
   MNVec4();
   ~MNVec4();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
};

///////////////////////////////////////////////////////////////////////////////
