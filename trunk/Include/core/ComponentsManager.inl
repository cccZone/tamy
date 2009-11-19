#ifndef _COMPONENTS_MANAGER_H
#error "This file can only be included from ComponentsManager.h"
#else

#include "core\SingletonsManager.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
ComponentsManager<Derrived>::ComponentsManager() 
: m_services(new SingletonsManager())
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
ComponentsManager<Derrived>::~ComponentsManager() 
{
   unsigned int count = m_comps.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_comps[i];
   }
   m_comps.clear();

   delete m_services; m_services = NULL;
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
void ComponentsManager<Derrived>::add(Component<Derrived>* component)
{
   try
   {
      component->initialize(*(dynamic_cast<Derrived*> (this)));
   }
   catch (std::exception&)
   {
      delete component;
      return;
   }
   m_comps.push_back(component);
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
template <typename T>
void ComponentsManager<Derrived>::registerService(T& service)
{
   bool result = m_services->setShared<T> (service);
   if (result == false)
   {
      throw std::runtime_error("Service is already registered");
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
template <typename T>
T& ComponentsManager<Derrived>::requestService()
{
   return m_services->shared<T> ();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derrived>
unsigned int ComponentsManager<Derrived>::getComponentsCount() const
{
   return m_comps.size();
}


///////////////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_MANAGER_H
