/// @file   core-Renderer/MNBool.h
/// @brief  a node that provides a bool value
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSBoolOutput;

///////////////////////////////////////////////////////////////////////////////

class MNBool : public MaterialNode
{
   DECLARE_ALLOCATOR( MNBool, AM_ALIGNED_16 );
   DECLARE_CLASS()

private:
   // static data
   bool             m_value;

   // runtime data
   MSBoolOutput*    m_output;
 
public:
   /**
    * Constructor.
    */
   MNBool();
   ~MNBool();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
