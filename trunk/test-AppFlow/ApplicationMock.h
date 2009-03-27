#pragma once

#include "core-AppFlow\Application.h"
#include "core\Point.h"


///////////////////////////////////////////////////////////////////////////////

class ApplicationMock : public Application
{
private:
   bool m_initialized;
   float m_timeElapsed;
   int m_receivedSignal;

public:
   ApplicationMock(const std::string& name) 
         : Application(name), 
         m_initialized(false), 
         m_timeElapsed(0),
         m_receivedSignal(-1)
   {}

   bool isInitialized() const {return m_initialized;}

   float getTimeElapsed() 
   {
      float result = m_timeElapsed;
      m_timeElapsed = 0;
      return result;
   }

   int getReceivedSignal() 
   {
      int result = m_receivedSignal;
      m_receivedSignal = -1;
      return result;
   }

   bool isKeyPressed(unsigned char keyCode) const
   {
      return context().isKeyPressed(keyCode);
   }

   const Point& getMousePos() const
   {
      return context().getMousePos();
   }

   void sendSignal(const std::string& receiverApp, int signalId)
   {
      context().signal(*this, receiverApp, signalId);
   }

   void sendSignal(int signalId)
   {
      context().signal(*this, signalId);
   }

   void finish()
   {
      context().signal(*this, ON_EXIT);
   }

   void update(float timeElapsed)
   {
      m_timeElapsed = timeElapsed;
   }

   void initialize(Renderer& renderer, ResourceManager& resourceManager)
   {
      m_initialized = true;
   }

   void deinitialize() 
   {
      m_initialized = false;
   }

   void notify(const std::string& senderApp, int signalCode)
   {
      m_receivedSignal = signalCode;
   }
};

///////////////////////////////////////////////////////////////////////////////
