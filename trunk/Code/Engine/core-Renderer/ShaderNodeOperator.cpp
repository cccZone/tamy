#include "core-Renderer/ShaderNodeOperator.h"
#include "core-Renderer/PixelShaderConstant.h"
#include "core-Renderer/PixelShader.h"
#include "core-Renderer/RenderingPipelineNode.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/Renderer.h"


///////////////////////////////////////////////////////////////////////////////

ShaderNodeOperator::ShaderNodeOperator( RenderingPipelineNode& hostNode )
   : m_hostNode( hostNode )
   , m_shader( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

ShaderNodeOperator::~ShaderNodeOperator()
{
   resetShader();
}

///////////////////////////////////////////////////////////////////////////////

void ShaderNodeOperator::setShader( PixelShader& shader )
{
   resetShader();

   m_shader = &shader;

   const std::vector< PixelShaderConstant* >& constants = m_shader->getConstants();
   unsigned int count = constants.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      m_constants.push_back( new ConstantDef( *constants[i] ) );
      m_constants.back()->setHostNode( &m_hostNode );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ShaderNodeOperator::resetShader()
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

// TODO: przekazywanie RuntimeDataBuffer z RenderingPipeline'u do REnderState'u jest TYLKO po to, zeby shaderNodeOperation
// moglo wyciagnac info z inputu - do zmiany, bo to jest niepotrzebne gdziekolwiek indziej i wprowadza zamotke
void ShaderNodeOperator::onPreRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   if ( m_shader )
   {
      RCBindPixelShader* comm = new ( renderer() ) RCBindPixelShader( *m_shader );

      // set the shader constants
      unsigned int count = m_constants.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         ConstantDef* def = m_constants[i];
         def->m_constant.setValue( *comm, *def->m_input, data );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void ShaderNodeOperator::onPostRender( Renderer& renderer, RuntimeDataBuffer& data ) const
{
   if ( m_shader )
   {
      new ( renderer() ) RCUnbindPixelShader( *m_shader );
   }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ShaderNodeOperator::ConstantDef::ConstantDef( PixelShaderConstant& constant ) 
   : m_constant( constant )
   , m_input( NULL )
   , m_hostNode( NULL )
{}

///////////////////////////////////////////////////////////////////////////////

void ShaderNodeOperator::ConstantDef::setHostNode( RenderingPipelineNode* hostNode )
{
   const std::string& inputName = m_constant.getName();

   if ( m_hostNode )
   {
      // remove it from the old node
      m_hostNode->removeInput( inputName );
   }

   m_hostNode = hostNode;

   if ( m_hostNode )
   {
      // add it to the new node
      RPNodeInput* input = m_hostNode->findInput( inputName );
      if ( input && !m_constant.doesTypeMatch( *input ) )
      {
         m_hostNode->removeInput( inputName );
         input = NULL;
      }

      if ( !input )
      {
         input = m_constant.createRPNInput();
         m_hostNode->defineInput( input );
      }

      m_input = input;
   }
}

///////////////////////////////////////////////////////////////////////////////
