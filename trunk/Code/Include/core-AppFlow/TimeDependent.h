#pragma once


///////////////////////////////////////////////////////////////////////////////

class TimeDependent
{
public:
   virtual ~TimeDependent() {}

   virtual void update(float timeElapsed) = 0;
};

///////////////////////////////////////////////////////////////////////////////

template<typename T>
class TTimeDependent : public TimeDependent
{
private:
   T& m_instance;

public:
   TTimeDependent(T& instance) : m_instance(instance) {}

   void update(float timeElapsed)
   {
      m_instance.update(timeElapsed);
   }
};

///////////////////////////////////////////////////////////////////////////////
