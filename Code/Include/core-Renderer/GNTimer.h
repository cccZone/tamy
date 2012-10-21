/// @file   core-Renderer/GNTimer.h
/// @brief  a node that provides current time value
#pragma once

#include "core-Renderer/GeometryShaderNode.h"
#include "core/Timer.h"


///////////////////////////////////////////////////////////////////////////////

class GSFloatOutput;

///////////////////////////////////////////////////////////////////////////////

class GNTimer : public GeometryShaderNode
{
   DECLARE_ALLOCATOR( GNTimer, AM_ALIGNED_16 );
   DECLARE_CLASS()

private:
   // static data
   float             m_period;
   float             m_amplitude;

   // runtime data
   GSFloatOutput*    m_time;
   GSFloatOutput*    m_sinTime;
   CTimer            m_timer;
 
public:
   /**
    * Constructor.
    */
   GNTimer();
   ~GNTimer();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // GeometryShaderNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const GeometryEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
