/// @file   core-Renderer/MNPixelShader.h
/// @brief  a node that operates a pixel shader
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/PixelShaderNodeOperator.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

class MNPixelShader : public MaterialNode
{
DECLARE_CLASS()

private:
   // static data
   PixelShader*                           m_shader;

   // runtime data
   PixelShaderNodeOperator< MaterialNode >*    m_shaderNode;

public:
   /**
    * Constructor.
    */
   MNPixelShader();
   ~MNPixelShader();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();
   void onPrePropertyChanged( ReflectionProperty& property );
   void onPropertyChanged( ReflectionProperty& property );

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void onGraphLoaded();
   void preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;
   void postRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const;

};

///////////////////////////////////////////////////////////////////////////////
