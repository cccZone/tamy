#pragma once

#include "core\Node.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This class represents a node that marks the position of the sound
 * listener in the 3D scene (his ears to be exact ;)
 */
class SoundListener : public Node
{
public:
   virtual ~SoundListener() {}

   void onAccept(NodeVisitor& visitor);

   virtual void update() = 0;

protected:
    SoundListener() : Node("soundListener") {}
};

///////////////////////////////////////////////////////////////////////////////
