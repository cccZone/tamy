#include "core-AppFlow\KeysStatusManager.h"
#include "core-AppFlow\KeysStatusModel.h"
#include "core-AppFlow\KeyStatusHandler.h"


///////////////////////////////////////////////////////////////////////////////

KeysStatusManager::KeysStatusManager( KeysStatusModel& model )
   : KEYS_COUNT( 256 )
   , m_model( model )
   , m_statuses( KEYS_COUNT, KeyStatus() )
   , m_smashTimeLimit( 0.1f )
   , m_time( 0 )
{
}

///////////////////////////////////////////////////////////////////////////////

KeysStatusManager::~KeysStatusManager()
{
   removeAllHandlers();
}

///////////////////////////////////////////////////////////////////////////////

void KeysStatusManager::update( float timeElapsed ) 
{
   /*
    Call this method if you want to use the manager - it updates its
    internal status, making it track changes in key states.
   
    You can define sensitivity by calling it more or less frequently.
    The pause between subsequent calls will define how sensitive the
    checks will be.
    For instance - if you want to consider a 'Key Smash' to be an event
    where you press a key for max. 0.2 sec, then you need to call this method
    at most every 0.2 sec.
    */

   m_time += timeElapsed;

   for ( unsigned int keyCode = 0; keyCode < KEYS_COUNT; ++keyCode )
   {
      KeyState oldState = m_statuses[keyCode].state;
      KeyState newState = oldState;

      if ( m_model.isKeyPressed( keyCode ) == true )
      {
         switch( oldState )
         {
         case KEY_FREE:
         case KEY_RELEASED: 
            {
               newState = KEY_PRESSED; 
               m_statuses[keyCode].timePressed = m_time; 
               break;
            }
         case KEY_PRESSED:    
            {
               float timeSincePressStart = m_time - m_statuses[keyCode].timePressed;
               if ( timeSincePressStart >= m_smashTimeLimit )
               {
                  newState = KEY_HELD; 
               }
               break;
            }
         }
      }
      else
      {
         switch( oldState )
         {
         case KEY_PRESSED:   
         case KEY_HELD:
            { 
               newState = KEY_RELEASED; 
               break;
            }
         }
      }

      executeAction( keyCode, oldState, newState );

      m_statuses[keyCode].state = newState;
   }
}

///////////////////////////////////////////////////////////////////////////////

void KeysStatusManager::addHandler( KeyStatusHandler* handler )
{
   if ( handler == NULL )
   {
      throw std::invalid_argument( "NULL pointer instead a KeyStatusHandler instance" );
   }

   m_handlers.push_back( handler );
}

///////////////////////////////////////////////////////////////////////////////

void KeysStatusManager::removeAllHandlers()
{
   unsigned int handlersCount = m_handlers.size();
   for ( unsigned int j = 0; j < handlersCount; ++j )
   {
      delete m_handlers[j];
   }
   m_handlers.clear();
}

///////////////////////////////////////////////////////////////////////////////

void KeysStatusManager::executeAction( unsigned char c, KeyState oldState, KeyState newState )
{
   unsigned int handlersCount = m_handlers.size();

   for ( unsigned int i = 0; i < handlersCount; ++i )
   {
      if ( oldState == KEY_RELEASED && newState == KEY_PRESSED )
      {
         m_handlers[i]->keySmashed( c );
      }
      else if ( oldState == KEY_PRESSED && newState == KEY_HELD )
      {
         m_handlers[i]->keyHeld( c );
      }
      else if ( oldState == KEY_HELD && newState == KEY_HELD )
      {
         m_handlers[i]->keyHeld( c );
      }
      else if ( ( oldState == KEY_HELD || oldState == KEY_PRESSED ) && newState == KEY_RELEASED )
      {
         m_handlers[i]->keyReleased( c );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void KeysStatusManager::setSmashTimeLimit( float limit )
{
   m_smashTimeLimit = limit;
}

///////////////////////////////////////////////////////////////////////////////

float KeysStatusManager::getSmashTimeLimit() const
{
   return m_smashTimeLimit;
}

///////////////////////////////////////////////////////////////////////////////
