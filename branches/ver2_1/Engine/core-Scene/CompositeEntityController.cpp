#include "core-Scene\CompositeEntityController.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

CompositeEntityController::~CompositeEntityController()
{
   unsigned int count = m_controllers.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_controllers[i];
   }
   m_controllers.clear();
}

///////////////////////////////////////////////////////////////////////////////

void CompositeEntityController::add(EntityController* controller)
{
   if (controller == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an EntityController instance");
   }

   m_controllers.push_back(controller);
}

///////////////////////////////////////////////////////////////////////////////

void CompositeEntityController::update(float timeElapsed)
{
   unsigned int count = m_controllers.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_controllers[i]->update(timeElapsed);
   }
}

///////////////////////////////////////////////////////////////////////////////
