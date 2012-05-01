/// @file   core-Renderer/GNVec4.h
/// @brief  a node that provides a vector value
#pragma once

#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

class GSVec4Output;

///////////////////////////////////////////////////////////////////////////////

class GNVec4 : public GeometryShaderNode
{
   DECLARE_CLASS()

private:
   // static data
   float             m_x;
   float             m_y;
   float             m_z;
   float             m_w;

   // runtime data
   GSVec4Output*    m_output;
 
public:
   /**
    * Constructor.
    */
   GNVec4();
   ~GNVec4();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // GeometryShaderNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const GeometryEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
