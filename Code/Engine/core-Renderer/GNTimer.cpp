#include "core-Renderer/GNTimer.h"
#include "core-Renderer/GeometrySockets.h"
#include "core-Renderer/GeometryEntity.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( GNTimer );
   PARENT( GeometryShaderNode );
   PROPERTY_EDIT( "period", float, m_period );
   PROPERTY_EDIT( "amplitude", float, m_amplitude );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

GNTimer::GNTimer()
   : m_period( 1.0f )
   , m_amplitude( 1.0f )
   , m_time( new GSFloatOutput( "t" ) )
   , m_sinTime( new GSFloatOutput( "sin(t * p) * a" ) )
{
   std::vector< GBNodeOutput< GeometryShaderNode >* > outputs;
   outputs.push_back( m_time );
   outputs.push_back( m_sinTime );

   defineOutputs( outputs );
}

///////////////////////////////////////////////////////////////////////////////

GNTimer::~GNTimer()
{
   m_time = NULL;
   m_sinTime = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void GNTimer::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_time = DynamicCast< GSFloatOutput >( findOutput( "t" ) );
   m_sinTime = DynamicCast< GSFloatOutput >( findOutput( "sin(t * p) * a" ) );
}

///////////////////////////////////////////////////////////////////////////////

void GNTimer::preRender( Renderer& renderer, const GeometryEntity& entity ) const
{
   float currTime = (float)m_timer.getCurrentTime();
   float sinTime = sin( currTime * m_period ) * m_amplitude;

   m_time->setValue( entity.data(), currTime );
   m_sinTime->setValue( entity.data(), sinTime );
}

///////////////////////////////////////////////////////////////////////////////
