#pragma once

#include "core-AppFlow\TimeEvent.h"
#include "core-AppFlow\TimeControllerTrack.h"


///////////////////////////////////////////////////////////////////////////////

class SpeedChangeEventMock : public TimeEvent
{
private:
   float m_newSpeed;
   TimeControllerTrack& m_track;

public:
   SpeedChangeEventMock(const std::string& id,
                        float newSpeed,
                        TimeControllerTrack& track)
         : TimeEvent(id),
         m_newSpeed(newSpeed),
         m_track(track)
   {}

   void execute()
   {
      m_track.setSpeed(m_newSpeed);
   }
};

///////////////////////////////////////////////////////////////////////////////
