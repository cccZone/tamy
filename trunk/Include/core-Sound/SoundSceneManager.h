#pragma once

#include "core\AbstractSceneManager.h"
#include "core\TNodesVisitor.h"
#include "core\Array.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

class Sound3D;
class SoundListener;
typedef Sound3D* Sound3DP;

///////////////////////////////////////////////////////////////////////////////

/**
 * This scene manager manages the audio representation of a 3D scene
 */
class SoundSceneManager : public AbstractSceneManager,
                          public SceneAspectManager<SoundListener>,
                          public SceneAspectManager<Sound3D>
{
private:
   SoundListener* m_activeListener;

   Array<Sound3D*> m_soundsArr;

public:
   SoundSceneManager(unsigned int maxElemsPerSector = 64, float worldSize = 1000);
   virtual ~SoundSceneManager();

   /**
    * The method returns true if there's an active listener set for the scene
    */
   bool hasActiveListener() const;

   /**
    * This method returns an instance of a sound listener currently activated
    * in this scene
    *
    * @throw std::logic_error if there's no listener set
    */
   SoundListener& getActiveListener();

   /**
    * The method checks which sounds that could be heard before
    * aren't now, and which of those that weren't are
    */
   Array<Sound3D*>& update(Array<Sound3D*>& soundsToDisable, 
                           Array<Sound3D*>& soundsToEnable);

protected:
   void onAdd(Sound3D& sound);
   void onRemove(Sound3D& sound);

   /**
   * A 3D sound in order to be perceived requires someone
   * to listen to it - this someone needs to have a position in the 3D world etc.
   * This someone is represented by the SoundListener
   */
   void onAdd(SoundListener& sound);
   void onRemove(SoundListener& sound);
};

///////////////////////////////////////////////////////////////////////////////
