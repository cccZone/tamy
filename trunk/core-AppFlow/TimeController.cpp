#include "core-AppFlow\TimeController.h"
#include <stdexcept>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////

TimeController::TimeController()
{
}

///////////////////////////////////////////////////////////////////////////////

TimeController::~TimeController()
{
   unsigned int count = m_tracks.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_tracks[i];
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::update(float timeElapsed)
{
   unsigned int count = m_tracks.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_tracks[i]->update(timeElapsed);
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::add(const std::string& trackID)
{
   std::vector<TimeControllerTrack*>::iterator it =
      std::find_if(m_tracks.begin(), m_tracks.end(), FindTrack(trackID));

   if (it == m_tracks.end())
   {
      m_tracks.push_back(new TimeControllerTrack(trackID));
   }
}

///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack& TimeController::get(const std::string& trackID)
{
   std::vector<TimeControllerTrack*>::iterator it =
      std::find_if(m_tracks.begin(), m_tracks.end(), FindTrack(trackID));

   if (it == m_tracks.end())
   {
      throw std::runtime_error(std::string("Track ") + trackID + " doesn't exist");
   }

   return **it;
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::resetAll()
{
   unsigned int count = m_tracks.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_tracks[i]->reset();
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

bool TimeController::FindTrack::operator()(const TimeControllerTrack* track) const
{
   return (track->getID() == m_id);
}

///////////////////////////////////////////////////////////////////////////////