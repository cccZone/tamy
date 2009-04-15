#pragma once

#include "core-Sound\SoundChannel.h"
#include <list>


///////////////////////////////////////////////////////////////////////////////

class SoundChannelMock : public SoundChannel
{
private:
   std::list<DWORD> m_buffers;
   bool m_wasReassigned;

public:
   SoundChannelMock(int id) 
         : SoundChannel(id), m_wasReassigned(false) {}

   void update() {}

   DWORD getNumBytesToPlay()
   {
      if (m_buffers.size() > 0)
      {
         return m_buffers.front();
      }
      else
      {
         return 0;
      }
   }

   void simulateBufferEnd()
   {
      m_buffers.pop_front();
   }

   int getActiveBuffersCount() const
   {
      return (int)m_buffers.size();
   }

   bool wasReassigned()
   {
      bool result = m_wasReassigned;
      m_wasReassigned = false;
      return result;
   }

protected:
   void addDataToPlayBuffer(char* data, DWORD size, 
                            const std::string& format, DWORD freq)
   {
      m_buffers.push_back(size);
   }

   void cleanBuffers()
   {
      m_wasReassigned = true;
      m_buffers.clear();
   }

   void startPlaying()
   {
   }

   void stopPlaying()
   {
   }
};

///////////////////////////////////////////////////////////////////////////////
