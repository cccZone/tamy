#include "core-AI\ControllersManager.h"
#include "core-AI\Controller.h"
#include <stdexcept>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

ControllersManager::~ControllersManager()
{
   unsigned int count = m_controllers.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_controllers[i];
   }
   m_controllers.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ControllersManager::addController(Controller* newController)
{
   if (newController == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an Controller instance");
   }
   m_controllers.push_back(newController);
}

///////////////////////////////////////////////////////////////////////////////

void ControllersManager::removeController(Controller& controller)
{
   ControllersColl::iterator it = std::find(m_controllers.begin(),
                                            m_controllers.end(),
                                            &controller);
   if (it == m_controllers.end())
   {
      throw std::invalid_argument("Trying to remove an unregistered controller");
   }
   
   delete *it;
   *it = NULL;
   unsigned int idx = it - m_controllers.begin();
   m_toBeRemoved.push_back(idx);
}

///////////////////////////////////////////////////////////////////////////////

void ControllersManager::update(float timeElapsed)
{
   unsigned int count = m_controllers.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_controllers[i] != NULL)
      {
         m_controllers[i]->update(timeElapsed);
      }
   }

   // remove the deleted controllers from the collection
   count = m_toBeRemoved.size();
   if (count > 0)
   {
      std::sort(m_toBeRemoved.begin(), m_toBeRemoved.end());
      for (unsigned int i = 0; i < count; ++i)
      {
         m_controllers.erase(m_controllers.begin() + (m_toBeRemoved[i] - i));
      }
      m_toBeRemoved.clear();
   }
}

///////////////////////////////////////////////////////////////////////////////
