#pragma once

#include <d3dx9.h>
#include "core\Color.h"


///////////////////////////////////////////////////////////////////////////////

struct Particle
{
   float lifeSpan;
   float timeToLive;
   float size;
   D3DXVECTOR3 position;
   D3DXVECTOR3 velocity;
   Color color;

   Particle() 
         : lifeSpan(0),
         timeToLive(0), 
         size(0),
         position(0, 0, 0),
         velocity(0, 0, 0),
         color(0, 0, 0, 0)
   {}

};

///////////////////////////////////////////////////////////////////////////////
