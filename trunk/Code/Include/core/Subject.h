#pragma once

#include <vector>
#include "core\Observer.h"


///////////////////////////////////////////////////////////////////////////////

/**
 * This is the observation subject that allows to attach a
 * a single type of observer. @see Observer class for more details
 * about realized observation model
 */
template<typename SubjectType, typename MessageEnum>
class Subject
{
private:
   std::vector<Observer<SubjectType, MessageEnum>*> m_observers;

public:
   virtual ~Subject() {}

   void attachObserver(Observer<SubjectType, MessageEnum>& observer)
   {
      m_observers.push_back(&observer);
      observer.update(dynamic_cast<SubjectType&> (*this));
   }

   void detachObserver(Observer<SubjectType, MessageEnum>& observer)
   {
      for (typename std::vector<Observer<SubjectType, MessageEnum>*>::iterator it = m_observers.begin();
           it != m_observers.end(); ++it)
      {
         if (*it == &observer)
         {
            m_observers.erase(it);
            break;
         }
      }
   }

protected:
   void notify( const MessageEnum& msg )
   {
      unsigned int count = m_observers.size();
      for (unsigned int i = 0; i < count; ++i)
      {
         m_observers[i]->update( static_cast< SubjectType& >( *this ), msg );
      }
   }

};

///////////////////////////////////////////////////////////////////////////////

