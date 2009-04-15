#pragma once

#include <list>


///////////////////////////////////////////////////////////////////////////////

class Sound;
class SoundChannel;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface represents a sound device - capable of playing, mixing
 * and doing other low level stuff with the sound
 */
class SoundDevice
{
private:
   int m_numBuffersUsed;
   std::list<SoundChannel*> m_activeChannels;

public:
   SoundDevice(int numBuffersUsed = 1);
   virtual ~SoundDevice();

   /**
    * The method returns the number describing how many channels
    * can be used by sounds
    */
   virtual int getChannelsCount() const = 0;

   /**
    * This method retrieves a sound channed
    */
   virtual SoundChannel& getChannel(int channelIdx) = 0;

   /**
    * The method assigns a sound to a free channel, allowing it to be played
    *
    * @throw std::runtime_error if there are no channels left or if the sound
    *                           already has a channel assigned
    */
   SoundChannel& activateSound(Sound& sound);

   /**
    * The method releases all currently occupied channels
    * on the device, effectively stopping all the sounds being played at the moment
    */
   void releaseAllChannels();

   /**
    * The method returns a list of all currently occupied channels
    */
   const std::list<SoundChannel*>& getActiveChannels() const {return m_activeChannels;}

   /**
    * Call this method in the main program loop to ensure continuous
    * sound and proper channels management
    */
   void update();
};

///////////////////////////////////////////////////////////////////////////////
