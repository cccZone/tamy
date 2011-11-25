/// @file   core-Renderer/MNPixelShader.h
/// @brief  a node that operates a pixel shader
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core-Renderer/ShaderNodeOperator.h"


///////////////////////////////////////////////////////////////////////////////

class PixelShader;

///////////////////////////////////////////////////////////////////////////////

class MNPixelShader : public MaterialNode
{
DECLARE_CLASS( MNPixelShader )

private:
   // static data
   PixelShader*                           m_shader;

   // runtime data
   ShaderNodeOperator< MaterialNode >*    m_shaderNode;

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
   void onPrePropertyChanged( Property& property );
   void onPropertyChanged( Property& property );

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, RuntimeDataBuffer& data ) const;
   void postRender( Renderer& renderer, RuntimeDataBuffer& data ) const;

};

///////////////////////////////////////////////////////////////////////////////
