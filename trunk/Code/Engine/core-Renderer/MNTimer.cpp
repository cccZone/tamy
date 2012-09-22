#include "core-Renderer/MNTimer.h"
#include "core-Renderer/MaterialSockets.h"
#include "core-Renderer/MaterialInstance.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( MNTimer );
   PARENT( MaterialNode );
   PROPERTY_EDIT( "period", float, m_period );
   PROPERTY_EDIT( "amplitude", float, m_amplitude );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

MNTimer::MNTimer()
   : m_period( 1.0f )
   , m_amplitude( 1.0f )
   , m_time( new MSFloatOutput( "t" ) )
   , m_sinTime( new MSFloatOutput( "sin(t * p) * a" ) )
{
   std::vector< GBNodeOutput< MaterialNode >* > outputs;
   outputs.push_back( m_time );
   outputs.push_back( m_sinTime );

   defineOutputs( outputs );
}

///////////////////////////////////////////////////////////////////////////////

MNTimer::~MNTimer()
{
   m_time = NULL;
   m_sinTime = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MNTimer::onObjectLoaded()
{
   __super::onObjectLoaded();

   // find the existing inputs
   m_time = DynamicCast< MSFloatOutput >( findOutput( "t" ) );
   m_sinTime = DynamicCast< MSFloatOutput >( findOutput( "sin(t * p) * a" ) );
}

///////////////////////////////////////////////////////////////////////////////

void MNTimer::preRender( Renderer& renderer, const MaterialInstance& instance, const MaterialEntity& entity ) const
{
   float currTime = (float)m_timer.getCurrentTime();
   float sinTime = sin( currTime * m_period ) * m_amplitude;

   m_time->setValue( instance.data(), currTime );
   m_sinTime->setValue( instance.data(), sinTime );
}

///////////////////////////////////////////////////////////////////////////////
