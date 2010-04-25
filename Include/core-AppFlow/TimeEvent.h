#pragma once

#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This interface represents a possibility of an action being executed 
 * at the specific point in time on a time track
 */
class TimeEvent
{
private:
   std::string m_id;

public:
   virtual ~TimeEvent() {}

   const std::string getID() const {return m_id;}

   virtual void execute() = 0;

protected:
   TimeEvent(const std::string& id) : m_id(id) {}
};

///////////////////////////////////////////////////////////////////////////////

template<typename T>
class TTimeEvent : public TimeEvent
{
private:
   T& m_instance;

public:
   TTimeEvent(const std::string& id, T& instance) 
         : TimeEvent(id), m_instance(instance) 
   {}

   void execute()
   {
      m_instance.execute();
   }
};

///////////////////////////////////////////////////////////////////////////////
