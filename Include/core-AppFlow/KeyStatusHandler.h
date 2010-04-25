#pragma once

/// @file   core-AppFlow\KeyStatusHandler.h
/// @brief  handler that will react to key state change


///////////////////////////////////////////////////////////////////////////////

/**
 * This handler operates from the KeysStatusManager context and will be
 * activated (an applicable method will be called) as soon as the status
 * of the observed key changes in one of the following ways:
 *
 * - key is smashed  (pressed and immediately released)
 * - key is held     (kept pressed for a while)
 * - key is released (released after being pressed for a while)
 */
class KeyStatusHandler
{
public:
   virtual ~KeyStatusHandler() {}

   /**
    * This method will be called when a key is pressed 
    * and immediately released.
    *
    * @param keyCode    code of the affected key
    */
   virtual void keySmashed(unsigned char keyCode) = 0;

   /**
    * This method will be called when a key is pressed and kept that way
    * for a while.
    *
    * @param keyCode    code of the affected key
    */
   virtual void keyHeld(unsigned char keyCode) = 0;

   /**
    * This method will be called when a key is released after being kept
    * pressed for a while.
    *
    * @param keyCode    code of the affected key
    */
   virtual void keyReleased(unsigned char keyCode) = 0;
};

///////////////////////////////////////////////////////////////////////////////
