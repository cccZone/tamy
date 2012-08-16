#include "core-Renderer/DepthBufferDescriptor.h"
#include "core-Renderer/DepthBuffer.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( DepthBufferDescriptor );
   PARENT( ReflectionObject );
   PROPERTY( std::string, m_id );
   PROPERTY( unsigned int, m_width );
   PROPERTY( unsigned int, m_height );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

DepthBufferDescriptor::DepthBufferDescriptor()
   : m_width( 1024 )
   , m_height( 1024 )
{
}

///////////////////////////////////////////////////////////////////////////////

DepthBufferDescriptor::~DepthBufferDescriptor()
{
}

///////////////////////////////////////////////////////////////////////////////

void DepthBufferDescriptor::initialize( RuntimeDataBuffer& runtimeData ) const
{
   runtimeData.registerVar< DepthBuffer* >( m_depthBuffer );
   runtimeData[m_depthBuffer] = new DepthBuffer( m_width, m_height );
}

///////////////////////////////////////////////////////////////////////////////

void DepthBufferDescriptor::deinitialize( RuntimeDataBuffer& runtimeData ) const
{
   delete runtimeData[m_depthBuffer];
   runtimeData[m_depthBuffer] = NULL;
}

///////////////////////////////////////////////////////////////////////////////
