#pragma once

#include <windows.h>
#include <iostream>
#include "core\Serializable.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

struct Face
{
   USHORT idx[3];
   DWORD subsetID;

   Face()
   {
      this->idx[0] = this->idx[1] = this->idx[2] = -1;
      this->subsetID = -1;
   }

   Face(USHORT i0, USHORT i1, USHORT i2, DWORD subsetID)
   {
      this->idx[0] = i0; this->idx[1] = i1; this->idx[2] = i2;
      this->subsetID = subsetID;
   }

   void save(Serializer& serializer)
   {
      serializer.saveInt(idx[0]);
      serializer.saveInt(idx[1]);
      serializer.saveInt(idx[2]);
      serializer.saveLong(subsetID);
   }

   void load(Serializer& serializer)
   {
      idx[0] = serializer.loadInt();
      idx[1] = serializer.loadInt();
      idx[2] = serializer.loadInt();
      subsetID = serializer.loadLong();
   }
};

///////////////////////////////////////////////////////////////////////////////
