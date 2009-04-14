#pragma once

#include "core-Sound\Sound.h"


///////////////////////////////////////////////////////////////////////////////

class SoundMock : public Sound
{
private:
   DWORD m_offset;
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

      m_offset = 0;
   }

   ~SoundMock()
   {
      m_bufSize = 0;

      delete [] m_someBuf;
      m_someBuf = NULL;

      m_offset = 0;
   }

   DWORD getDataOffset() const
   {
      return m_offset;
   }

   void setDataOffset(DWORD pos)
   {
      m_offset = pos;
   }

   DWORD getBytesPerSec() const
   {
      return m_bytesPerSec;
   }

   DWORD getBlockAlignment() const
   {
      return m_blockAlignment;
   }

   DWORD getData(char* data, DWORD bufSize)
   {
      DWORD toRead = m_bufSize - m_offset;
      if (bufSize < toRead) toRead = bufSize;
      memcpy(data, m_someBuf + m_offset, toRead);
      m_offset += toRead;

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
