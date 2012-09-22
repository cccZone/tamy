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

   /**
    * A custom method for this node. It will prepare a render command that sets up and binds
    * the vertex shader this node represents.
    *
    * @param renderer
    * @param entity
    */
   RCBindVertexShader* createRenderCommand( Renderer& renderer, const GeometryEntity& entity ) const;

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
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
