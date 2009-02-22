#pragma once

class CTimer
{
private:
   __int64         m_lastTime;
   float           m_timeScale;
   float           m_timeElapsed;

   unsigned long   m_frameRate;
   unsigned long   m_fpsFrameCount;
	float           m_fpsTimeElapsed;

public:
   CTimer(void);
   ~CTimer(void);

   void tick(void);

   float getTimeElapsed() const;

   double getCurrentTime() const;

   unsigned long getFrameRate() const;
};
