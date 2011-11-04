#pragma once


///////////////////////////////////////////////////////////////////////////////

class UpdatableMock : public TimeDependent
{
private:
   int m_timesCalled;
   float m_lastTime;

public:
   UpdatableMock() : m_timesCalled(0), m_lastTime(0) {}

   void update(float timeElapsed)
   {
      m_lastTime = timeElapsed;
      ++m_timesCalled;
   }

   int getTimesCalled()
   {
      int result = m_timesCalled;
      m_timesCalled = 0;
      return result;
   }

   float getLastTime() const {return m_lastTime;}
};

///////////////////////////////////////////////////////////////////////////////
