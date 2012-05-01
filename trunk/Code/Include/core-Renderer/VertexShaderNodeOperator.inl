#ifndef _VERTEX_SHADER_NODE_OPERATOR_H
#error "This file can only be included from VertexShaderNodeOperation.h"
#else

#include "core-Renderer/VertexShaderNodeOperator.h"
#include "core-Renderer/VertexShaderConstant.h"
#include "core-Renderer/VertexShader.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/ShaderCompiler.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VertexShaderNodeOperator< TNode >::VertexShaderNodeOperator( TNode& hostNode )
   : m_hostNode( hostNode )
   , m_shader( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VertexShaderNodeOperator< TNode >::~VertexShaderNodeOperator()
{
   resetShader();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VertexShaderNodeOperator< TNode >::setShader( VertexShader& shader )
{
   resetShader();

   m_shader = &shader;

   ShaderCompiler compiler;
   std::vector< VertexShaderConstant< TNode >* > constants;
   compiler.compileVertexShaderConstants( m_shader->getScript(), m_shader->getEntryFunctionName().c_str(), constants );

   unsigned int count = constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_constants.push_back( new ConstantDef( constants[i] ) );
      m_constants.back()->setHostNode( &m_hostNode );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VertexShaderNodeOperator< TNode >::resetShader()
{
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_constants[i]->setHostNode( NULL );
      delete m_constants[i];
   }
   m_constants.clear();

   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
RCBindVertexShader& VertexShaderNodeOperator< TNode >::bindShader( Renderer& renderer, RuntimeDataBuffer& data )
{
   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_shader );

   // set the shader constants
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      VertexShaderConstant< TNode >* constant = m_constants[i]->m_constant;
      GBNodeInput< TNode >* input = m_constants[i]->m_input;
      constant->setValue( *comm, *input, data );
   }

   return *comm;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VertexShaderNodeOperator< TNode >::ConstantDef::ConstantDef( VertexShaderConstant< TNode >* constant ) 
   : m_constant( constant )
   , m_input( NULL )
   , m_hostNode( NULL )
{}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
VertexShaderNodeOperator< TNode >::ConstantDef::~ConstantDef()
{
   delete m_constant;
   m_constant = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void VertexShaderNodeOperator< TNode >::ConstantDef::setHostNode( TNode* hostNode )
{
   const std::string& inputName = m_constant->getName();

   if ( m_hostNode )
   {
      // remove it from the old node
      m_hostNode->removeInput( inputName );
   }

   m_hostNode = hostNode;

   if ( m_hostNode )
   {
      // add it to the new node
      GBNodeInput< TNode >* input = m_hostNode->findInput( inputName );
      const ReflectionType* constantDataType = m_constant->getDataType();
      const ReflectionType* inputDataType = input->getDataType();
      ASSERT_MSG( constantDataType && inputDataType, "Vertex shader constant or input type not registered with the reflection system" );

      if ( input && !constantDataType->isExactlyA( *inputDataType ) )
      {
         m_hostNode->removeInput( inputName );
         input = NULL;
      }

      if ( !input )
      {
         input = hostNode->createInput( *constantDataType, inputName );
         hostNode->defineInput( input );
         ASSERT_MSG( input != NULL, "No input available that's able to marshal the specified data type" );
      }

      m_input = input;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _VERTEX_SHADER_NODE_OPERATOR_H
