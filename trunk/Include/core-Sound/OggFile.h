#pragma once

#include "core-Sound\Sound.h"
#include <windows.h>
#include <stdio.h>
#include <string>


///////////////////////////////////////////////////////////////////////////////

class OggFile : public Sound  
{
private:

public:
   OggFile(const std::string& fileName);
	~OggFile();

   DWORD getDataOffset() const;
   void setDataOffset(DWORD pos);

   DWORD getData(char* data, DWORD bufSize);
   std::string getFormat() const;
   unsigned int getFrequency() const;

   DWORD getBytesPerSec() const;
   DWORD getBlockAlignment() const;
};

///////////////////////////////////////////////////////////////////////////////
