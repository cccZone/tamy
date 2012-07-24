/// @file   core-Renderer/GNVertexShader.h
/// @brief  a node that operates a vertex shader
#pragma once

#include "core-Renderer/GeometryShaderNode.h"
#include "core-Renderer/VertexShaderNodeOperator.h"


///////////////////////////////////////////////////////////////////////////////

class VertexShader;

///////////////////////////////////////////////////////////////////////////////

class GNVertexShader : public GeometryShaderNode
{
DECLARE_CLASS()

private:
   // static data
   VertexShader*                                      m_shader;

   // runtime data
   VertexShaderNodeOperator< GeometryShaderNode >*    m_shaderNode;

public:
   /**
    * Constructor.
    */
   GNVertexShader();
   ~GNVertexShader();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPrePropertyChanged( ReflectionProperty& property );
   void onPropertyChanged( ReflectionProperty& property );

   // -------------------------------------------------------------------------
   // GeometryShaderNode implementation
   // -------------------------------------------------------------------------
   void onGraphLoaded();
   void preRender( Renderer& renderer, const GeometryEntity& entity ) const;
   void postRender( Renderer& renderer, const GeometryEntity& entity ) const;

};

///////////////////////////////////////////////////////////////////////////////
