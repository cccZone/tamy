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
   std::vector< VertexShaderConstant< TNode >* > allConstants;

   // let's compile all techniques to extract all possible shader constants used
   uint techniquesCount = shader.getTechniquesCount();
   for ( uint techniqueIdx = 0; techniqueIdx < techniquesCount; ++techniqueIdx )
   {
      const std::string& techniqueEntryFunc = m_shader->getEntryFunctionName( techniqueIdx );
      std::vector< VertexShaderConstant< TNode >* > constants;

      compiler.compileVertexShaderConstants( m_shader->getScript(), techniqueEntryFunc.c_str(), constants );

      uint constantsCount = constants.size();
      uint allConstantsCount = allConstants.size();
      for ( uint constantIdx = 0; constantIdx < constantsCount; ++constantIdx )
      {
         const std::string& checkedConstantName = constants[constantIdx]->getName();

         // look for a similar constant - browse through the constants that were already added to the 'allConstants' list
         // only, since the new ones are bound to be unique
         bool isNew = true;
         for ( uint mainConstantIdx = 0; mainConstantIdx < allConstantsCount; ++mainConstantIdx )
         {
            if ( allConstants[mainConstantIdx]->getName() == checkedConstantName )
            {
               // already have it
               delete constants[constantIdx];
               isNew = false;
               break;
            }
         }

         if ( isNew )
         {
            // this is a new constant - add it to the list
            allConstants.push_back( constants[constantIdx] );
         }
      }
   }

   unsigned int count = allConstants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_constants.push_back( new ConstantDef( allConstants[i] ) );
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
void VertexShaderNodeOperator< TNode >::filterSockets( std::vector< std::string >& inOutSocketNames ) const
{
   // create sockets for new constants
   unsigned int constantsCount = m_constants.size();
   for ( unsigned int constIdx = 0; constIdx < constantsCount; ++constIdx )
   {
      // first - locate the corresponding input and remove it from the list
      const std::string& constantName = m_constants[constIdx]->m_constant->getName();
      uint socketsCount = inOutSocketNames.size();
      for ( uint socketIdx = 0; socketIdx < socketsCount; ++socketIdx )
      {
         if ( inOutSocketNames[socketIdx] == constantName )
         {
            // found it - this socket corresponds to an existing constant, so we're 
            // crossing it off the list
            inOutSocketNames.erase( inOutSocketNames.begin() + socketIdx );
            break;
         }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
RCBindVertexShader* VertexShaderNodeOperator< TNode >::bindShader( Renderer& renderer, RuntimeDataBuffer& data )
{
   RCBindVertexShader* comm = new ( renderer() ) RCBindVertexShader( *m_shader, renderer );

   // set the shader constants
   unsigned int count = m_constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      VertexShaderConstant< TNode >* constant = m_constants[i]->m_constant;
      GBNodeInput< TNode >* input = m_constants[i]->m_input;
      constant->setValue( *comm, *input, data );
   }

   return comm;
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
      const ReflectionType* constantDataType = m_constant->getDataType();
      
      GBNodeInput< TNode >* input = m_hostNode->findInput( inputName );
      if ( input )
      {
         // the new node already has such input - check if its type matches our type. If not remove it
         const ReflectionType* inputDataType = input->getDataType();
         if ( !constantDataType->isExactlyA( *inputDataType ) )
         {
            m_hostNode->removeInput( inputName );
            input = NULL;
         }
      }

      // at this point if we don't have an input, we need to create a new one
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
