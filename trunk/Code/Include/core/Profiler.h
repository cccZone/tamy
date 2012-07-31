/// @file   core/Profiler.h
/// @brief  profiling tool
#pragma once

#include "core\types.h"
#include "core\Array.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class CTimer;

///////////////////////////////////////////////////////////////////////////////

/**
 * Simple engine profiling tool.
 *
 * It's a singleton, so you can access it from any place you like.
 *
 * You can register a timer and then time a part of your code by embracing
 * it with `start` and `end` method calls.
 *
 * You can embed timing sections within other timing sections. The profiler
 * will keep track of those dependencies, but it will still track time for each of
 * the sections independently ( for accuracy and reliability reasons ).
 */
class Profiler
{
public:
   struct Timer
   {
      std::string          m_name;
      double               m_activationTimestamp;
      double               m_timeElapsed;

      Timer( const std::string& name ) : m_name( name ), m_activationTimestamp( -1 ), m_timeElapsed( 0 ) {}
   };

   struct Trace 
   {
      uint                 m_parentTimerId;
      uint                 m_timerId;
   };
private:
   static Profiler         s_theInstance;

   const uint              MAX_TRACE_LENGTH;

   CTimer*                 m_engineTimer;

   Array< Timer* >         m_timers;
   Array< Trace* >         m_traces;

   bool                    m_active;
   uint                    m_stackTraceIdx;
   uint                    m_tracesCount;

   uint                    m_parentTimerIdx;

public:
   ~Profiler();

   /**
    * Returns the singleton instance of the profiler.
    */
   inline static Profiler& getInstance() { return s_theInstance; }

   // -------------------------------------------------------------------------
   // Timers management
   // -------------------------------------------------------------------------
   /**
    * Registers a new timer with the profiler.
    *
    * @param  timerName
    * @return timerId
    */
   uint registerTimer( const std::string& timerName );

   /**
    * Returns the name of the timer.
    *
    * @param timerId.
    */
   const std::string& getName( uint timerId ) const;

   // -------------------------------------------------------------------------
   // Profiling API
   // -------------------------------------------------------------------------

   /**
    * Call this at the beginning of every engine frame to reset the measures from the previous frame.
    */
   void beginFrame();

   /**
    * Call this at the end of every frame to hand out the profiling results
    */
   void endFrame();

   /**
    * Activates the specified timer.
    *
    * @param timerId
    */
   void start( uint timerId );

   /**
    * Deactivates the specified timer, counting the time it was running
    * and storing the result for future queries.
    *
    * @param timerId
    */
   void end( uint timerId );

   /**
    * Returns the number of recorded traces.
    */
   inline uint getTracesCount() const { return m_tracesCount; }

   /**
    * Returns the specified trace details.
    *
    * @param traceIdx
    */
   inline const Trace& getTrace( uint traceIdx ) const { return *m_traces[traceIdx]; }


   /**
    * Returns the number of timers.
    */
   inline uint getTimersCount() const { return m_timers.size(); }

   /**
    * Returns the amount of time the timer was running this frame.
    *
    * @param timerId
    */
   inline double getTimeElapsed( uint timerId ) const { return m_timers[timerId - 1]->m_timeElapsed; }

private:
   /**
    * Restricted default constructor.
    */
   Profiler();

   /**
    * Restricted copy constructor.
    */
   Profiler( const Profiler& );

   /**
    * This operation is restricted.
    */
   void operator=( const Profiler& profiler ) {}
};

///////////////////////////////////////////////////////////////////////////////

/**
 * A helper class that profiles the scope it's been instantiated in.
 */
struct ScopedProfiler
{
   uint        m_timerId;
   Profiler&   m_profiler;

   ScopedProfiler( uint timerId )
      : m_timerId( timerId )
      , m_profiler( Profiler::getInstance() )
   {
      m_profiler.start( m_timerId );
   }

   ~ScopedProfiler()
   {
      m_profiler.end( m_timerId );
   }
};

///////////////////////////////////////////////////////////////////////////////
// Helper macros
///////////////////////////////////////////////////////////////////////////////

/**
 * Wrap a function declaration in this macro, and it will instantly start profiling
 * the entire method.
 */
#define PROFILED() \
   struct __FunctionProfiler \
   { \
      uint m_timerId; \
      __FunctionProfiler( const char* name ) \
      { \
         m_timerId = Profiler::getInstance().registerTimer( name ); \
      } \
   }; \
   static __FunctionProfiler profilerDecl( __FUNCTION__ ); \
   ScopedProfiler __profiler( profilerDecl.m_timerId );
   
///////////////////////////////////////////////////////////////////////////////
