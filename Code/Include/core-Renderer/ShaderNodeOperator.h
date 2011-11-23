/// @file   core-Renderer/ShaderNodeOperator.h
/// @brief  Operator for nodes that should be parametrized according to the pixel shader they are using.
#ifndef _SHADER_NODE_OPERATOR_H
#define _SHADER_NODE_OPERATOR_H

#include <vector>


///////////////////////////////////////////////////////////////////////////////

class PixelShader;
class PixelShaderConstant;
class RenderingPipelineNode;
class Renderer;
class RuntimeDataBuffer;
class RPNodeInput;
class RCBindPixelShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class provides a pixel shader with a set of node connections
 * necessary for it to have its global constants set.
 */
template< typename TNode >
class ShaderNodeOperator
{
private:
   struct ConstantDef
   {
      PixelShaderConstant&             m_constant;
      RPNodeInput*                     m_input;
      TNode*                           m_hostNode;

      ConstantDef( PixelShaderConstant& constant );
      void setHostNode( TNode* hostNode );
   };

private:
   TNode&                              m_hostNode;
   PixelShader*                        m_shader;
   std::vector< ConstantDef* >         m_constants;

public:
   /**
    * Constructor.
    *
    * @param hostNode
    */
   ShaderNodeOperator( TNode& hostNode );
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
    * Propagates the data from the input to the assigned shader constant
    *
    * @param renderer
    * @param data       runtime data buffer the inputs are using
    *
    * @return           rendering command that binds the pixel shader
    */
   RCBindPixelShader& bindShader( Renderer& renderer, RuntimeDataBuffer& data );
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/ShaderNodeOperator.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _SHADER_NODE_OPERATOR_H
