#pragma once

#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

class SoundSceneManager;
class SoundDevice;
class Sound3D;

///////////////////////////////////////////////////////////////////////////////

/**
 * Just as the graphical renderer converts the scene nodes into 
 * a visiual representation, the SoundRenderer converts the applicable
 * scene nodes into an audio representation
 */
class SoundRenderer
{
private:
   SoundDevice& m_soundDevice;
   Array<Sound3D*> m_soundsToDisable;
   Array<Sound3D*> m_soundsToEnable;

public:
   SoundRenderer(SoundDevice& soundDevice);
   ~SoundRenderer();

   /**
    * The method renders the contents of an audio scene on the sound device
    */
   void render(SoundSceneManager& soundScene);
};

///////////////////////////////////////////////////////////////////////////////
