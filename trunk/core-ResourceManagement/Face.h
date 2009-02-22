#pragma once

#include <windows.h>


///////////////////////////////////////////////////////////////////////////////

template<typename index_type>
struct Face
{
   index_type idx[3];
   DWORD subsetID;

   Face(index_type i0, index_type i1, index_type i2, DWORD subsetID)
   {
      this->idx[0] = i0; this->idx[1] = i1; this->idx[2] = i2;
      this->subsetID = subsetID;
   }
};

///////////////////////////////////////////////////////////////////////////////
