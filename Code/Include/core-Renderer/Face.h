#pragma once

#include <windows.h>
#include <iostream>
#include "core\Serializable.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

struct Face
{
   USHORT idx[3];

   Face()
   {
      this->idx[0] = this->idx[1] = this->idx[2] = -1;
   }

   Face(USHORT i0, USHORT i1, USHORT i2)
   {
      this->idx[0] = i0; this->idx[1] = i1; this->idx[2] = i2;
   }

   friend Serializer& operator<<(Serializer& serializer, Face& face)
   {
      serializer << face.idx[0];
      serializer << face.idx[1];
      serializer << face.idx[2];
      return serializer;
   }
};

///////////////////////////////////////////////////////////////////////////////
