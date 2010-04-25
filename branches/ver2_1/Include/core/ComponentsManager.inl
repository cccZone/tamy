#ifndef _COMPONENTS_MANAGER_H
#error "This file can only be included from ComponentsManager.h"
#else

#include "core\SingletonsManager.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
ComponentsManager<Derived>::ComponentsManager() 
: m_services(new SingletonsManager())
{
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
ComponentsManager<Derived>::~ComponentsManager() 
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

template <typename Derived>
void ComponentsManager<Derived>::add(Component<Derived>* component)
{
   try
   {
      component->initialize(*(dynamic_cast<Derived*> (this)));
   }
   catch (std::exception&)
   {
      delete component;
      return;
   }
   m_comps.push_back(component);
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
template <typename T>
void ComponentsManager<Derived>::registerService(T& service)
{
   bool result = m_services->setShared<T> (service);
   if (result == false)
   {
      throw std::runtime_error("Service is already registered");
   }
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
template <typename T>
T& ComponentsManager<Derived>::requestService()
{
   return m_services->shared<T> ();
}

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
unsigned int ComponentsManager<Derived>::getComponentsCount() const
{
   return m_comps.size();
}


///////////////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_MANAGER_H
