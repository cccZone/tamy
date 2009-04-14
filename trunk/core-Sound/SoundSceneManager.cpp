#include "core-Sound\SoundSceneManager.h"
#include "core-Sound\Sound3D.h"
#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

SoundSceneManager::~SoundSceneManager()
{
}

///////////////////////////////////////////////////////////////////////////////

Sound3D& SoundSceneManager::addSoundSource(Node& nodeToAttachSoundTo)
{
   Sound3D* source = new Sound3D("source");
   nodeToAttachSoundTo.addChild(source);
   
   return *source;
}

///////////////////////////////////////////////////////////////////////////////
