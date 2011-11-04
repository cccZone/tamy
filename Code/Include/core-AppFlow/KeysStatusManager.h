#pragma once

/// @file   core-AppFlow\KeysStatusManager.h
/// @brief  tool for handling 3 different key states (smashed, pressed
///         and released)

#include <vector>
#include <map>
#include "core-AppFlow/TimeDependent.h"


///////////////////////////////////////////////////////////////////////////////

class KeysStatusModel;
class KeyStatusHandler;

///////////////////////////////////////////////////////////////////////////////

/**
 * Tool for handling 3 different key states (smashed, pressed and released).
 */
class KeysStatusManager : public TimeDependent
{
private:
   enum KeyState
   {
      KEY_FREE,
      KEY_PRESSED,
      KEY_HELD,
      KEY_RELEASED
   };

   struct KeyStatus
   {
      KeyState state;
      float timePressed;

      KeyStatus() : state(KEY_FREE), timePressed(0) {}
   };

private:
   const unsigned int KEYS_COUNT;

   KeysStatusModel& m_model;
   typedef std::vector<KeyStatusHandler*> Handlers;

   typedef std::map<unsigned char, Handlers> Keys;
   Handlers m_handlers;

   typedef std::vector<KeyStatus> Statuses;
   Statuses m_statuses;

   float m_smashTimeLimit;
   float m_time;

public:
   /**
    * Constructor.
    *
    * @param model   model with key statuses
    */
   KeysStatusManager(KeysStatusModel& model);

   ~KeysStatusManager();

   /**
    * This method adds a new event handler that will react to changes
    * in states of the keys.
    *
    * @param handler    handler that should be called when the state
    *                   of the key changes
    */
   void addHandler(KeyStatusHandler* handler);

   /**
    * Removes all attached handlers.
    */
   void removeAllHandlers();

   /**
    * Sets a new time limit after which if the key is still pressed, it
    * will be considered held, not smashed.
    *
    * @param limit   time value (in seconds)
    */
   void setSmashTimeLimit(float limit);

   /**
    * Returns currently set smash time limit.
    *
    * @return        smash time limit (in seconds)
    */
   float getSmashTimeLimit() const;

   // ----------------------------------------------------------------------------
   // TimeDependent implementation
   // ----------------------------------------------------------------------------
   void update(float timeElapsed);

private:
   void executeAction(unsigned char c, KeyState oldState, KeyState newState);
};

///////////////////////////////////////////////////////////////////////////////
