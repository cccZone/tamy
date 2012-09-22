/// @file   core-Renderer/MNVec4.h
/// @brief  a node that provides a vector value
#pragma once

#include "core-Renderer/MaterialNode.h"


///////////////////////////////////////////////////////////////////////////////

class MSVec4Output;

///////////////////////////////////////////////////////////////////////////////

class MNVec4 : public MaterialNode
{
   DECLARE_CLASS()

private:
   // static data
   Vector            m_vector;

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
   void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
