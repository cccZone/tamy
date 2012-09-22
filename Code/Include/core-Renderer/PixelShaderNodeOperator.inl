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
   // delete constants
   uint count = m_constants.size();
   for ( uint i = 0; i < count; ++i )
   {
      delete m_constants[i];
   }
   m_constants.clear();

   // set the new shader
   m_shader = &shader;

   // create shader constants
   const std::vector< ShaderConstantDesc >& constantsDescriptions = shader.getConstantsDescriptions();
   std::vector< PixelShaderConstant< TNode >* > constants;
   buildConstantsSockets( constantsDescriptions, constants );

   // create inputs for the constants
   // add it to the new node
   uint constantsCount = constants.size();

   std::vector< GBNodeInput< TNode >* > inputs;
   inputs.resize( constantsCount );

   for ( uint i = 0; i < constantsCount; ++i )
   {
      PixelShaderConstant< TNode >* constant = constants[i];
      const ReflectionType* constantDataType = constant->getDataType();

      GBNodeInput< TNode >* input = m_hostNode.createInput( *constantDataType, constant->getName() );
      ASSERT_MSG( input != NULL, "No input available that's able to marshal the specified data type" );

      inputs[i] = input;
   }

   // define the inputs on the host node
   m_hostNode.redefineInputs( inputs );

   // create a map of constants and corresponding inputs
   for ( unsigned int i = 0; i < constantsCount; ++i )
   {
      PixelShaderConstant< TNode >* constant = constants[i];
      GBNodeInput< TNode >* input = m_hostNode.findInput( constant->getName() );
      m_constants.push_back( new ConstantDef( constant, input ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PixelShaderNodeOperator< TNode >::resetShader()
{
   // delete constants
   uint count = m_constants.size();
   for ( uint i = 0; i < count; ++i )
   {
      delete m_constants[i];
   }
   m_constants.clear();

   // clear all inputs
   m_hostNode.removeAllInputs();

   m_shader = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
void PixelShaderNodeOperator< TNode >::filterSockets( std::vector< std::string >& inOutSocketNames ) const
{
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
RCBindPixelShader& PixelShaderNodeOperator< TNode >::bindShader( Renderer& renderer, RuntimeDataBuffer& data )
{
   RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader, renderer );

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

template< typename TNode >
void PixelShaderNodeOperator< TNode >::buildConstantsSockets( const std::vector< ShaderConstantDesc >& constants, std::vector< PixelShaderConstant< TNode >* >& outConstantsSockets ) const
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
               outConstantsSockets[i] = new PSCBool< TNode >( constant.m_name.c_str(), false );
               break;
            }

         case ShaderConstantDesc::CT_Int:
            {
               outConstantsSockets[i] = new PSCInt< TNode >( constant.m_name.c_str(), 0 );
               break;
            }

         case ShaderConstantDesc::CT_Float:
            {
               outConstantsSockets[i] = new PSCFloat< TNode >( constant.m_name.c_str(), 0.0f );
               break;
            }

         case ShaderConstantDesc::CT_Matrix:
            {
               outConstantsSockets[i] = new PSCMatrix< TNode >( constant.m_name.c_str() );
               break;
            }

         case ShaderConstantDesc::CT_String:
            {
               outConstantsSockets[i] = new PSCString< TNode >( constant.m_name.c_str() );
               break;
            }

         case ShaderConstantDesc::CT_Texture:
            {
               outConstantsSockets[i] = new PSCTexture< TNode >( constant.m_name.c_str() );
               break;
            }

         case ShaderConstantDesc::CT_Vec4:
            {
               outConstantsSockets[i] = new PSCVec4< TNode >( constant.m_name.c_str() );
               break;
            }

         default:
            {
               ASSERT_MSG( false, "Unknown pixel shader constant type encountered" );
               break;
            }
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderNodeOperator< TNode >::ConstantDef::ConstantDef( PixelShaderConstant< TNode >* constant, GBNodeInput< TNode >* input ) 
   : m_constant( constant )
   , m_input( input )
{}

///////////////////////////////////////////////////////////////////////////////

template< typename TNode >
PixelShaderNodeOperator< TNode >::ConstantDef::~ConstantDef()
{
   delete m_constant;
   m_constant = NULL;
}

///////////////////////////////////////////////////////////////////////////////


#endif // _PIXEL_SHADER_NODE_OPERATOR_H
