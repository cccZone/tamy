#pragma once

#include <string>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a single sound. A sound is nothing more than
 * data - on its own it can't do much. But when registered with a SoundDevice,
 * it can be played, mixed with other sounds and so on
 */
class Sound
{
public:
   virtual ~Sound() {}

   /**
    * This method returns the current position inside the sound data buffer
    * we're reading from
    */
   virtual DWORD getDataOffset() const = 0;

   /**
    * This method allows to adjust the data reading position
    */
   virtual void setDataOffset(DWORD pos) = 0;

   /**
    * The method retrieves the data from the sound file to the preallocated buffer
    * from the current reading position (which can be obtained using @see getDataOffset 
    * method)
    *
    * @param data - preallocated buffer the data will be stored in
    * @param bufSize - size of the preallocated buffer - that's how much
    *                  data we want read from the file
    *
    * @return the actural number of bytes read from the file
    */
   virtual DWORD getData(char* data, DWORD bufSize) = 0;

   /** 
    * The sound format of the file
    */
   virtual std::string getFormat() const = 0;

   /**
    * The frequency with which the file should be read
    */
   virtual unsigned int getFrequency() const = 0;

   /**
    * The acerage amount of bytes that should be played per second for the file
    */
   virtual DWORD getBytesPerSec() const = 0;

   /**
    * Sound data block alignment value
    */
   virtual DWORD getBlockAlignment() const = 0;
};

///////////////////////////////////////////////////////////////////////////////
