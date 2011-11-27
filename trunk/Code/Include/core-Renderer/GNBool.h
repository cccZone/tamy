/// @file   core-Renderer/GNBool.h
/// @brief  a node that provides a bool value
#pragma once

#include "core-Renderer/GeometryShaderNode.h"


///////////////////////////////////////////////////////////////////////////////

class GSBoolOutput;

///////////////////////////////////////////////////////////////////////////////

class GNBool : public GeometryShaderNode
{
   DECLARE_CLASS( GNBool )

private:
   // static data
   bool             m_value;

   // runtime data
   GSBoolOutput*    m_output;
 
public:
   /**
    * Constructor.
    */
   GNBool();
   ~GNBool();

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
