#pragma once

#include <string>
#include <windows.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class Sound;

///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a channel on a sound device that can host 
 * the playing of a single sound
 */
class SoundChannel
{
private:
   int m_id;
   DWORD m_sampleLength;
   
   int m_nextFreeBuf;
   std::vector<char*> m_data;

   bool m_isPlaying;
   bool m_looped;

   Sound* m_sound;
   std::string m_currSoundFormat;
   DWORD m_currSoundFreq;

   DWORD m_currPeriodicPos;
   DWORD m_soundLength;

public:
   virtual ~SoundChannel();

   /**
    * Each channel has a unique ID assigned
    */
   int getID() {return m_id;}

   /**
    * The method returns true if the channel has asound assigned and
    * the sound is being played
    */
   bool isPlaying() const;

   /**
    * The method signals the system that we want the sound to be played
    */
   void play();

   /**
    * The method signals the system that we don't want to play the sound
    */
   void stop();

   /**
    * The method allows to play the sound in a looped or one-time only manner
    */
   void setLooped(bool enable);

   /**
    * This method is called from the SoundDevice during a periodic update
    */
   virtual void update() = 0;

   /**
    * This method is called by the SoundDevice when it deems there's
    * not enough data in the channel and wants to stream some more
    * to maintain the uniterrupted sound quality
    */
   void loadNextSample();

   /**
    * The method should return the number of sound buffers queued in the
    * channel sound queue
    */
   virtual int getActiveBuffersCount() const = 0;

   /**
    * The method returns true if the channel has a sound assigned
    */
   bool isBusy() const;


   /**
    * The method allows to assign a sound to a channel
    */
   void assignSound(Sound& sound);

   /**
    * The method allows to remove the sound from a channel, freeing it
    * and allowing to be used by another sound
    */
   void removeSound();

   /**
    * The method adjusts the sound sample size that will be queried from
    * the sound file
    */
   void setSampleLength(DWORD len);

   /**
    * Counterpart of the @see setSampleLength method
    */
   DWORD getSampleLength() const;

   
   /**
    * This method returns the current position inside the sound data buffer
    * we're reading from
    *
    * The position returned is specified in seconds
    */
   float getPosition() const;

   /**
    * This method allows to adjust the data reading position
    *
    * The position should be specified in seconds
    */
   void setPosition(float pos);

   /**
    * The method returns the lenght of the sound (in seconds)
    */
   float getSoundLength() const;

protected:
   SoundChannel(int id, int buffersCount);

   /**
    * The method should add new data to the channel's sound queue
    */
   virtual void addDataToPlayBuffer(char* data, DWORD size,
                                    const std::string& format, 
                                    DWORD freq) = 0;

   /**
    * The method should clean up the channel's sound queue
    */
   virtual void onCleanBuffers() = 0;

   /**
    * The method should issue an implementation specific command that will
    * start playing the sound
    */
   virtual void startPlaying() = 0;

   /**
    * The method should issue an implementation specific command that will
    * seize playing the sound
    */
   virtual void stopPlaying() = 0;

private:
   void setPosition(DWORD pos);
   void cleanBuffers();
};

///////////////////////////////////////////////////////////////////////////////
