#pragma once

#include "core-Sound\Sound.h"


///////////////////////////////////////////////////////////////////////////////

class SoundMock : public Sound
{
private:
   DWORD m_bufSize;
   char* m_someBuf;
   int m_bytesPerSec;
   int m_blockAlignment;

public:
   SoundMock(int bufSize = 16, int bytesPerSec = 0xffff, int blockAlignment = 1)
   {
      m_bufSize = bufSize;
      m_bytesPerSec = bytesPerSec;
      m_blockAlignment = blockAlignment;

      m_someBuf = new char[bufSize];
      for (int i = 0; i < bufSize; ++i)
      {
         m_someBuf[i] = i;
      }
   }

   ~SoundMock()
   {
      m_bufSize = 0;

      delete [] m_someBuf;
      m_someBuf = NULL;
   }

   DWORD getLength() const
   {
      return m_bufSize;
   }

   DWORD getBytesPerSec() const
   {
      return m_bytesPerSec;
   }

   DWORD getBlockAlignment() const
   {
      return m_blockAlignment;
   }

   DWORD getData(DWORD periodicPos, char* data, DWORD bufSize)
   {
      DWORD toRead = m_bufSize - periodicPos;
      if (bufSize < toRead) toRead = bufSize;
      memcpy(data, m_someBuf + periodicPos, toRead);

      return toRead;
   }

   std::string getFormat() const
   {
      return "someFormat";
   }

   unsigned int getFrequency() const
   {
      return 4800;
   }
};

///////////////////////////////////////////////////////////////////////////////
