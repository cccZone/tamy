/// @file   core-Renderer/MNFloat.h
/// @brief  a node that provides a float value
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSFloatOutput;

///////////////////////////////////////////////////////////////////////////////

class MNFloat : public MaterialNode
{
   DECLARE_CLASS( MNFloat )

private:
   // static data
   float             m_value;

   // runtime data
   MSFloatOutput*    m_output;
 
public:
   /**
    * Constructor.
    */
   MNFloat();
   ~MNFloat();

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
