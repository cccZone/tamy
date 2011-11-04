#include "core-AppFlow\TimeControllerTrack.h"
#include "core-AppFlow\TimeDependent.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack::TimeControllerTrack( const std::string& id )
      : m_id(id),
      m_speed(1),
      m_updateFreq(0),
      m_timeline(0),
      m_lastUpdate(0),
      m_nextUpdate(0)
{
}

///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack::~TimeControllerTrack()
{
   reset();
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::add( TimeDependent& object )
{
   // check if the object isn't already on the track
   unsigned int count = m_objects.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_objects[i] == &object )
      {
         // yup - it's already there
         return;
      }
   }

   m_objects.push_back( &object );
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::remove( TimeDependent& object )
{
   unsigned int count = m_objects.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( m_objects[i] == &object )
      {
         m_objects.erase( m_objects.begin() + i );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::setEvent(float time, TimeEvent* e)
{
   m_events.push_back(new Event(e, m_timeline + time));
   std::sort(m_events.begin(), m_events.end(), m_eventComparator);
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::removeFirstEvent(const std::string& id)
{
   for (std::vector<Event*>::iterator it = m_events.begin();
        it != m_events.end(); ++it)
   {
      if ((*it)->action->getID() == id)
      {
         delete *it;
         m_events.erase(it);
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::removeAllEvents(const std::string& id)
{
   for (std::vector<Event*>::iterator it = m_events.begin();
        it != m_events.end();)
   {
      if ((*it)->action->getID() == id)
      {
         delete *it;
         it = m_events.erase(it);
      }
      else
      {
          ++it;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::setSpeed(float multiplier)
{
   if (multiplier < 0) {multiplier = 0;}

   m_speed = multiplier;
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

   // execute all events times for which has come
   Event* currEvent = NULL;
   std::vector<Event*>::iterator eventIt = m_events.begin();
   while ((m_events.size() > 0) && ((currEvent = *eventIt)->time <= m_timeline))
   {
      eventIt = m_events.erase(eventIt);
      currEvent->action->execute();
      delete currEvent;
   }


   // update the objects on the track
   float timeForObjects = timeElapsed * m_speed;
   unsigned int count = m_objects.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_objects[i]->update(timeForObjects);
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeControllerTrack::reset()
{
   m_speed = 1;
   m_objects.clear();

   unsigned int count = m_events.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_events[i];
   }
   m_events.clear();
}

///////////////////////////////////////////////////////////////////////////////
