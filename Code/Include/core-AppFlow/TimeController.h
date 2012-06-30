/// @file   core-AppFlow\TimeController.h
/// @brief  tool for scheduling tasks in time
#pragma once


#include <vector>
#include <string>
#include "core-AppFlow\TimeControllerTrack.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * TimeController provides the means to update multiple 'updatable' classes
 * simultaneously in a timely fashion.
 *
 * It can have many time tracks defined, each running with a different speed.
 * Each 'updatable' can be added to such track, and thus effectively
 * be running with a different speed.
 *
 * It also provides the facility to limit the execution of a particular track
 * to a specified frame rate.
 */
class TimeController
{
private:
   class FindTrack
   {
   private:
      std::string m_id;

   public:
      FindTrack(const std::string& id) : m_id(id) {}

      bool operator()(const TimeControllerTrack* track) const;
   };

   TimeController*                           m_parentController;
   std::vector< TimeControllerTrack* >       m_tracks;
   std::vector< TimeController* >            m_childControllers;

public:
   /**
    * Constructor.
    */
   TimeController();

   /**
    * Constructor setting a controllers hierarchy.
    */
   TimeController( TimeController& parentController );
   ~TimeController();

   /**
    * Adds a new track.
    *
    * @param trackID
    */
   TimeControllerTrack& add( const std::string& trackID );

   /**
    * Returns a track with the specified ID.
    *
    * @param trackID
    */
   TimeControllerTrack& get( const std::string& trackID );

   /**
    * Removes a track with the specified ID
    *
    * @param trackID
    */
   void remove( const std::string& trackID );

   /**
    * Updates all registered tracks.
    *
    * @param timeElapsed
    */
   void update( float timeElapsed );

   /**
    * Resets all tracks by resetting their speeds and removing all
    * attached objects and events.
    */
   void resetAll();

protected:
   /**
    * Attaches a new child controller.
    *
    * @param child
    */
   void attach( TimeController& child );

   /**
    * Detaches a child controller.
    *
    * @param child
    */
   void detach( TimeController& child );

   /**
    * Removes the parent information.
    */
   void clearParent();
};

///////////////////////////////////////////////////////////////////////////////
