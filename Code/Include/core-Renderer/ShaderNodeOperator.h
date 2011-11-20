/// @file   core-Renderer/ShaderNodeOperator.h
/// @brief  Operator for nodes that should be parametrized according to the pixel shader they are using.
#pragma once

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class PixelShaderConstant;
class RPNodeInput;
class RenderingPipelineNode;
class Renderer;
class RuntimeDataBuffer;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class provides a pixel shader with a set of node connections
 * necessary for it to have its global constants set.
 */
class ShaderNodeOperator
{
private:
   struct ConstantDef
   {
      PixelShaderConstant&             m_constant;
      RPNodeInput*                     m_input;
      RenderingPipelineNode*           m_hostNode;

      ConstantDef( PixelShaderConstant& constant );
      void setHostNode( RenderingPipelineNode* hostNode );
   };

private:
   RenderingPipelineNode&        m_hostNode;
   PixelShader*                  m_shader;
   std::vector< ConstantDef* >   m_constants;

public:
   /**
    * Constructor.
    *
    * @param hostNode
    */
   ShaderNodeOperator( RenderingPipelineNode& hostNode );
   ~ShaderNodeOperator();

   /**
    * Sets a new shader for the node to use.
    *
    * @param shader
    */
   void setShader( PixelShader& shader );

   /**
    * Removes the previously set shader instance.
    */
   void resetShader();

   /**
    * Binds the shader and sets its global constants.
    *
    * @param renderer
    * @param data
    */
   void onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const;

   /**
    * Unbinds the shader.
    *
    * @param renderer
    * @param data
    */
   void onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const;

};

///////////////////////////////////////////////////////////////////////////////
