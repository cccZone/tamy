#include "core-Renderer/RPFloats2Vec4Node.h"
#include "core-Renderer/RenderingPipelineSockets.h"
#include "core-Renderer/RenderingPipelineMechanism.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( RPFloats2Vec4Node );
   PARENT( RenderingPipelineNode );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

RPFloats2Vec4Node::RPFloats2Vec4Node()
{
   m_dataInput[0] = new RPFloatInput( "x" );
   m_dataInput[1] = new RPFloatInput( "y" );
   m_dataInput[2] = new RPFloatInput( "z" );
   m_dataInput[3] = new RPFloatInput( "w" );
   m_output = new RPVec4Output( "Output" );

   std::vector< GBNodeInput< RenderingPipelineNode >* > inputs;
   inputs.push_back( m_dataInput[0] );
   inputs.push_back( m_dataInput[1] );
   inputs.push_back( m_dataInput[2] );
   inputs.push_back( m_dataInput[3] );
   defineInputs( inputs );

   defineOutput( m_output );
}

///////////////////////////////////////////////////////////////////////////////

RPFloats2Vec4Node::~RPFloats2Vec4Node()
{
   m_dataInput[0] = NULL;
   m_dataInput[1] = NULL;
   m_dataInput[2] = NULL;
   m_dataInput[3] = NULL;
   m_output = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void RPFloats2Vec4Node::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_dataInput[0] = DynamicCast< RPFloatInput >( findInput( "x" ) );
   m_dataInput[1] = DynamicCast< RPFloatInput >( findInput( "y" ) );
   m_dataInput[2] = DynamicCast< RPFloatInput >( findInput( "z" ) );
   m_dataInput[3] = DynamicCast< RPFloatInput >( findInput( "w" ) );
   m_output = DynamicCast< RPVec4Output >( findOutput( "Output" ) );
}

///////////////////////////////////////////////////////////////////////////////

void RPFloats2Vec4Node::onUpdate( RenderingPipelineMechanism& host ) const
{
   RuntimeDataBuffer& data = host.data();

   Vector vec;
   vec[0] = m_dataInput[0]->getValue( data );
   vec[1] = m_dataInput[1]->getValue( data );
   vec[2] = m_dataInput[2]->getValue( data );
   vec[3] = m_dataInput[3]->getValue( data );
   m_output->setValue( data, vec );
}

///////////////////////////////////////////////////////////////////////////////
