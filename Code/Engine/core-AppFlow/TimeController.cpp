#include "core-AppFlow\TimeController.h"
#include "core\Assert.h"
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////

TimeController::TimeController()
   : m_parentController( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

TimeController::TimeController( TimeController& parentController )
   : m_parentController( &parentController )
{
   m_parentController->attach( *this );
}

///////////////////////////////////////////////////////////////////////////////

TimeController::~TimeController()
{
   if ( m_parentController )
   {
      m_parentController->detach( *this );
      m_parentController = NULL;
   }

   unsigned int count = m_tracks.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_tracks[i];
   }

   // auto detach child controllers
   count = m_childControllers.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_childControllers[i]->clearParent();
   }
   m_childControllers.clear();
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::attach( TimeController& child )
{
   // make sure the controller's not already attached
   std::vector< TimeController* >::iterator it = std::find( m_childControllers.begin(), m_childControllers.end(), &child );
   if ( it == m_childControllers.end() )
   {
      m_childControllers.push_back( &child );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::detach( TimeController& child )
{
   std::vector< TimeController* >::iterator it = std::find( m_childControllers.begin(), m_childControllers.end(), &child );
   if ( it != m_childControllers.end() )
   {
      m_childControllers.erase( it );
   }
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::clearParent()
{
   m_parentController = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::update(float timeElapsed)
{
   unsigned int count = m_tracks.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      m_tracks[i]->update( timeElapsed );
   }

   count = m_childControllers.size();
   for( unsigned int i = 0; i < count; ++i )
   {
      m_childControllers[i]->update( timeElapsed );
   }
}

///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack& TimeController::add(const std::string& trackID)
{
   std::vector< TimeControllerTrack* >::iterator it = std::find_if( m_tracks.begin(), m_tracks.end(), FindTrack( trackID ) );

   TimeControllerTrack* track;
   if ( it == m_tracks.end() )
   {
      track = new TimeControllerTrack( trackID );
      m_tracks.push_back( track );
   }
   else
   {
      track = *it;
   }

   return *track;
}

///////////////////////////////////////////////////////////////////////////////

TimeControllerTrack& TimeController::get(const std::string& trackID)
{
   std::vector<TimeControllerTrack*>::iterator it = std::find_if(m_tracks.begin(), m_tracks.end(), FindTrack(trackID));

   if (it == m_tracks.end())
   {
      ASSERT_MSG( false, "Track doesn't exist");
   }

   return **it;
}

///////////////////////////////////////////////////////////////////////////////

void TimeController::remove(const std::string& trackID)
{
   std::vector<TimeControllerTrack*>::iterator it = std::find_if(m_tracks.begin(), m_tracks.end(), FindTrack(trackID));

   if (it != m_tracks.end())
   {
      delete *it;
      m_tracks.erase(it);
   }
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