/// @file   core-Renderer/MNTimer.h
/// @brief  a node that provides current time value
#pragma once

#include "core-Renderer/MaterialNode.h"
#include "core/Timer.h"


///////////////////////////////////////////////////////////////////////////////

class MSFloatOutput;

///////////////////////////////////////////////////////////////////////////////

class MNTimer : public MaterialNode
{
   DECLARE_CLASS()

private:
   // static data
   float             m_period;
   float             m_amplitude;

   // runtime data
   MSFloatOutput*    m_time;
   MSFloatOutput*    m_sinTime;
   CTimer            m_timer;
 
public:
   /**
    * Constructor.
    */
   MNTimer();
   ~MNTimer();

   // -------------------------------------------------------------------------
   // Object implementation
   // -------------------------------------------------------------------------
   void onObjectLoaded();

   // -------------------------------------------------------------------------
   // MaterialNode implementation
   // -------------------------------------------------------------------------
   void preRender( Renderer& renderer, const MaterialEntity& entity ) const;
};

///////////////////////////////////////////////////////////////////////////////
