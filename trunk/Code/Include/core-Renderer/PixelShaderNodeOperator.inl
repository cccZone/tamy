#ifndef _PIXEL_SHADER_NODE_OPERATOR_H
#error "This file can only be included from PixelShaderNodeOperation.h"
#else


#include "core-Renderer/PixelShaderNodeOperator.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/Renderer.h"
#include "core-Renderer/ShaderCompiler.h"


///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderNodeOperator< TNode >::PixelShaderNodeOperator( TNode& hostNode )
   : m_hostNode( hostNode )
   , m_shader( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderNodeOperator< TNode >::~PixelShaderNodeOperator()
{
   resetShader();
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PixelShaderNodeOperator< TNode >::setShader( PixelShader& shader )
{
   resetShader();

   m_shader = &shader;

   ShaderCompiler compiler;
   std::vector< PixelShaderConstant< TNode >* > constants;
   compiler.compilePixelShaderConstants( m_shader->getScript(), m_shader->getEntryFunctionName().c_str(), constants );

   unsigned int count = constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_constants.push_back( new ConstantDef( constants[i] ) );
      m_constants.back()->setHostNode( &m_hostNode );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PixelShaderNodeOperator< TNode >::resetShader()
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
 RCBindPixelShader& PixelShaderNodeOperator< TNode >::bindShader( Renderer& renderer, RuntimeDataBuffer& data )
{
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

   // set the shader constants
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      PixelShaderConstant< TNode >* constant = m_constants[i]->m_constant;
      GBNodeInput< TNode >* input = m_constants[i]->m_input;
      constant->setValue( *comm, *input, data );
   }

   return *comm;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderNodeOperator< TNode >::ConstantDef::ConstantDef( PixelShaderConstant< TNode >* constant ) 
   : m_constant( constant )
   , m_input( NULL )
   , m_hostNode( NULL )
{}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderNodeOperator< TNode >::ConstantDef::~ConstantDef()
{
   delete m_constant;
   m_constant = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PixelShaderNodeOperator< TNode >::ConstantDef::setHostNode( TNode* hostNode )
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
      Class constantDataType = m_constant->getDataType();
      if ( input && !constantDataType.isExactlyA( input->getDataType() ) )
      {
         m_hostNode->removeInput( inputName );
         input = NULL;
      }

      if ( !input )
      {
         input = hostNode->createInput( constantDataType, inputName );
         hostNode->defineInput( input );
         ASSERT_MSG( input != NULL, "No input available that's able to marshal the specified data type" );
      }

      m_input = input;
   }
}

///////////////////////////////////////////////////////////////////////////////

#endif // _PIXEL_SHADER_NODE_OPERATOR_H
