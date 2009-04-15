#pragma once

#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class Sound3D;
class SoundListener;
typedef Sound3D* Sound3DP;

///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager manages the audio representation of a 3D scene
 */
class SoundSceneManager
{
public:
   virtual ~SoundSceneManager() {}

   /**
    * A 3D sound in order to be perceived requires someone
    * to listen to it - this someone needs to have a position in the 3D world etc.
    * This someone is represented by the SoundListener
    */
   virtual void setActiveListener(SoundListener& listener) = 0;

   /**
    * The method returns true if there's an active listener set for the scene
    */
   virtual bool hasActiveListener() const = 0;

   /**
    * This method returns an instance of a sound listener currently activated
    * in this scene
    *
    * @throw std::logic_error if there's no listener set
    */
   virtual SoundListener& getActiveListener() = 0;

   /**
    * The method adds a new 3d sound source to the scene
    */
   virtual void add(Sound3D& sound) = 0;

   /**
    * The method checks which sounds that could be heard before
    * aren't now, and which of those that weren't are
    */
   virtual void calculateSoundsHearability() = 0;

   /**
    * The method returns an array of all the sound that can no longer
    * be heard from the listener's point of view
    *
    * An empty array is returned if no listener is set
    *
    * @param soundsCount - when the method returns, this reference
    *                      will contain the number of elements in the returned array
    */
   virtual Sound3DP* getSoundsToDisable(DWORD& soundsCount) = 0;

   /**
    * The method returns an array of all new sound that can be heard
    * from the listener's point of view
    *
    * An empty array is returned if no listener is set
    *
    * @param soundsCount - when the method returns, this reference
    *                      will contain the number of elements in the returned array
    */
   virtual Sound3DP* getSoundsToEnable(DWORD& soundsCount) = 0;
};

///////////////////////////////////////////////////////////////////////////////
