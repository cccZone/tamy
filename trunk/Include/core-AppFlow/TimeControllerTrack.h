#pragma once

#include <vector>
#include <string>
#include "core-AppFlow\TimeDependent.h"


///////////////////////////////////////////////////////////////////////////////

class TimeControllerTrack
{
private:
   std::string m_id;
   float m_multiplierDuration;
   float m_multiplier;
   float m_updateFreq;

   float m_timeline;
   float m_lastUpdate;
   float m_nextUpdate;

   std::vector<TimeDependent*> m_objects;

public:
   TimeControllerTrack(const std::string& id);
   ~TimeControllerTrack();

   const std::string& getID() const {return m_id;}

   void add(TimeDependent* object);

   void update(float timeElapsed);

   void setMultiplier(float multiplier, float duration);

   void setLimit(float timesPerSec);

   void reset();
};

///////////////////////////////////////////////////////////////////////////////
