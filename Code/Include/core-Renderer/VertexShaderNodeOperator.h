/// @file   core-Renderer/VertexShaderNodeOperator.h
/// @brief  Operator for nodes that should be parametrized according to the vertex shader they are using.
#ifndef _VERTEX_SHADER_NODE_OPERATOR_H
#define _VERTEX_SHADER_NODE_OPERATOR_H

#include <vector>
#include "core-Renderer/VertexShaderConstant.h"

///////////////////////////////////////////////////////////////////////////////

class VertexShader;
class Renderer;
class RuntimeDataBuffer;
class RCBindVertexShader;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class provides a vertex shader with a set of node connections
 * necessary for it to have its global constants set.
 */
template< typename TNode >
class VertexShaderNodeOperator
{
private:
   struct ConstantDef
   {
      VertexShaderConstant< TNode >*      m_constant;
      GBNodeInput< TNode >*               m_input;
      TNode*                              m_hostNode;

      ConstantDef( VertexShaderConstant< TNode >* constant );
      ~ConstantDef();
      void setHostNode( TNode* hostNode );
   };

private:
   TNode&                                 m_hostNode;
   VertexShader*                          m_shader;
   std::vector< ConstantDef* >            m_constants;

public:
   /**
    * Constructor.
    *
    * @param hostNode
    */
   VertexShaderNodeOperator( TNode& hostNode );
   ~VertexShaderNodeOperator();

   /**
    * Sets a new shader for the node to use.
    *
    * @param shader
    */
   void setShader( VertexShader& shader );

   /**
    * Removes the previously set shader instance.
    */
   void resetShader();

   /**
    * Filters the sockets that DON'T belong to the current set of pixel shader constants.
    *
    * @param inOutSocketName
    */
   void filterSockets( std::vector< std::string >& inOutSocketNames ) const;

   /**
    * Propagates the data from the input to the assigned shader constant
    *
    * @param renderer
    * @param data       runtime data buffer the inputs are using
    *
    * @return           rendering command that binds the pixel shader
    */
   RCBindVertexShader* bindShader( Renderer& renderer, RuntimeDataBuffer& data );

private:
   /**
    * This helper methods builds the actual node sockets for the specified shader constants.
    *
    * @param constants
    * @param outConstantsSockets
    */
   void buildConstantsSockets( const std::vector< ShaderConstantDesc >& constants, std::vector< VertexShaderConstant< TNode >* >& outConstantsSockets ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "core-Renderer/VertexShaderNodeOperator.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _VERTEX_SHADER_NODE_OPERATOR_H
