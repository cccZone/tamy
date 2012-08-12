/// @file   core/Log.h
/// @brief  logging utility
#pragma once

#include "core/dostream.h"


///////////////////////////////////////////////////////////////////////////////

struct Log
{
   static Log     s_theInstance;

   dostream       m_outputStream;
};

///////////////////////////////////////////////////////////////////////////////

/**
 * Basic logging macro - outputs the specified message to the system log.
 */
#define LOG( msg ) Log::s_theInstance.m_outputStream << msg;

///////////////////////////////////////////////////////////////////////////////

/**
 * Logs a message to the system log providing the specified condition is met.
 */
#define CONDITIONAL_LOG( condition, msg ) if ( condition ) { Log::s_theInstance.m_outputStream << msg; }

///////////////////////////////////////////////////////////////////////////////
