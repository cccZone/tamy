#include "core-AppFlow\TimeControllerTrack.h"
#include "core-AppFlow\TimeDependent.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack::TimeControllerTrack(const std::string& id)
      : m_id(id),
      m_multiplierDuration(0),
      m_multiplier(1),
      m_updateFreq(0),
      m_timeline(0),
      m_lastUpdate(0),
      m_nextUpdate(0)
{
}

///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack::~TimeControllerTrack()
{
   unsigned int count = m_objects.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_objects[i];
   }
}
///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::add(TimeDependent* object)
{
   if (object == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a TimeDependent instance");
   }

   m_objects.push_back(object);
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::setMultiplier(float multiplier, float duration)
{
   if (duration < 0) {duration = 0;}
   if (multiplier < 0) {multiplier = 0;}

   m_multiplierDuration = duration;
   m_multiplier = multiplier;
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::setLimit(float timesPerSec)
{
   m_updateFreq = 1.f / timesPerSec;
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::update(float timeElapsed)
{
   // determine if this is the right time to
   // run an update on this track, or should we wait
   m_timeline += timeElapsed;
   if (m_timeline >= m_nextUpdate)
   {
      timeElapsed = (m_timeline - m_lastUpdate);
      m_lastUpdate = m_timeline;

      if (m_updateFreq > 0)
      {
         while (m_nextUpdate <= m_timeline) 
         {
            m_nextUpdate += m_updateFreq;
         }
      }
      else
      {
         m_nextUpdate = m_timeline;
      }
   }
   else
   {
      return;
   }

   // change the speed of time according to the speed of the track
   if (m_multiplierDuration > 0)
   {
      m_multiplierDuration -= timeElapsed;
      timeElapsed *= m_multiplier;
   }

   // update the objects on the track
   unsigned int count = m_objects.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_objects[i]->update(timeElapsed);
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::reset()
{
   m_multiplierDuration = 0;
   m_multiplier = 1;
}

///////////////////////////////////////////////////////////////////////////////
