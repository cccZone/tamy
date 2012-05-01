/// @file   core-Renderer/GNFloat.h
/// @brief  a node that provides a float value
#pragma once

#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

class GSFloatOutput;

///////////////////////////////////////////////////////////////////////////////

class GNFloat : public GeometryShaderNode
{
   DECLARE_CLASS()

private:
   // static data
   float             m_value;

   // runtime data
   GSFloatOutput*    m_output;
 
public:
   /**
    * Constructor.
    */
   GNFloat();
   ~GNFloat();

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
