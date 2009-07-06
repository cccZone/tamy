#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

class Sound3D;
class SoundListener;
class Sound;

///////////////////////////////////////////////////////////////////////////////

/**
 * This abstract factory will create the implementation
 * specific versions of sound entities
 */
class SoundEntitiesFactory
{
public:
   virtual ~SoundEntitiesFactory() {}

   virtual Sound3D* createEmiter(const std::string& name, 
                                 bool dynamic,
                                 Sound& sound, 
                                 float hearingRadius) = 0;

   virtual SoundListener* createListener() = 0;
};

///////////////////////////////////////////////////////////////////////////////
