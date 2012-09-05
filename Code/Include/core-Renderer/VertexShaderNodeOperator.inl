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

   const std::vector< ShaderConstantDesc >& constantsDescriptions = shader.getConstantsDescriptions();
   std::vector< VertexShaderConstant< TNode >* > allConstants;
   buildConstantsSockets( constantsDescriptions, allConstants );

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

template< typename TNode >
void VertexShaderNodeOperator< TNode >::buildConstantsSockets( const std::vector< ShaderConstantDesc >& constants, std::vector< VertexShaderConstant< TNode >* >& outConstantsSockets ) const
{
   uint count = constants.size();
   outConstantsSockets.resize( count );

   for ( uint i = 0; i < count; ++i )
   {
      const ShaderConstantDesc& constant = constants[i];
      switch( constant.m_type )
      {
      case ShaderConstantDesc::CT_Bool:
         {
            outConstantsSockets[i] = new VSCBool< TNode >( constant.m_name.c_str(), false );
            break;
         }

      case ShaderConstantDesc::CT_Int:
         {
            outConstantsSockets[i] = new VSCInt< TNode >( constant.m_name.c_str(), 0 );
            break;
         }

      case ShaderConstantDesc::CT_Float:
         {
            outConstantsSockets[i] = new VSCFloat< TNode >( constant.m_name.c_str(), 0.0f );
            break;
         }

      case ShaderConstantDesc::CT_Matrix:
         {
            outConstantsSockets[i] = new VSCMatrix< TNode >( constant.m_name.c_str() );
            break;
         }

      case ShaderConstantDesc::CT_String:
         {
            outConstantsSockets[i] = new VSCString< TNode >( constant.m_name.c_str() );
            break;
         }

      case ShaderConstantDesc::CT_Texture:
         {
            outConstantsSockets[i] = new VSCTexture< TNode >( constant.m_name.c_str() );
            break;
         }

      case ShaderConstantDesc::CT_Vec4:
         {
            outConstantsSockets[i] = new VSCVec4< TNode >( constant.m_name.c_str() );
            break;
         }

      default:
         {
            ASSERT_MSG( false, "Unknown vertex shader constant type encountered" );
            break;
         }
      }
   }
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
