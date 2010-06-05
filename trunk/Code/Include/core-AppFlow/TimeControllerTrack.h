#pragma once

#include <vector>
#include <string>
#include "core-AppFlow\TimeDependent.h"
#include "core-AppFlow\TimeEvent.h"


///////////////////////////////////////////////////////////////////////////////

class TimeControllerTrack
{
private:
   struct Event
   {
      TimeEvent* action;
      float time;

      Event(TimeEvent* _action, float _time)
         : action(_action), time(_time)
      {}

      ~Event()
      {
         delete action;
         action = NULL;
      }
   };

   class EventComparator
   {
   public:
      bool operator()(const Event* rhs, const Event* lhs)
      {
         return rhs->time < lhs->time;
      }
   };

private:
   std::string m_id;
   float m_speed;
   float m_updateFreq;

   float m_timeline;
   float m_lastUpdate;
   float m_nextUpdate;

   std::vector<TimeDependent*> m_objects;
   std::vector<Event*> m_events;
   EventComparator m_eventComparator;

public:
   TimeControllerTrack(const std::string& id);
   ~TimeControllerTrack();

   const std::string& getID() const {return m_id;}

   /**
    * Adds a new time dependent object to the track.
    *
    * @param object
    */
   void add( TimeDependent* object );

   /**
    * Updates the state of the track.
    *
    * @param timeElapsed   time elapsed since the last time this method was called.
    */
   void update( float timeElapsed );

   /**
    * This allows to set the track's speed. The speed
    * determines what time value will the objects residing
    * on the track's time line receive.
    *
    * For instance - if the speed = 2 and one sec has passed,
    * the objects will be updated with the timeElapsed value = 2.
    *
    * Track's internal time line is not affected by this parameter.
    * This means that the events are not influenced by the track's speed.
    * It considers only TimeDependent objects !!!
    */
   float getSpeed() const {return m_speed;}

   void setSpeed(float multiplier);

   void setLimit(float timesPerSec);

   float getTime() const {return m_timeline;}

   /** 
    * This method sets a new event to be executed somewhere in time
    * @param time specifies the period of time from the current track
    * time the event should take place
    */
   void setEvent(float time, TimeEvent* e);

   /**
    * This method removes the first occurance
    * of this event instance (there can be many)
    */
   void removeFirstEvent(const std::string& id);

   /**
    * This method removes all occurances
    * of this event instance (there can be many)
    */
   void removeAllEvents(const std::string& id);

   void reset();
};

///////////////////////////////////////////////////////////////////////////////
