#ifndef _COMPONENTS_MANAGER_H
#define _COMPONENTS_MANAGER_H

/// @file   core\ComponentsManager.h
/// @brief  base for all component oriented classes

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class SingletonsManager;

///////////////////////////////////////////////////////////////////////////////

template <typename Derived>
class ComponentsManager
{
private:
   std::vector<Component<Derived>*> m_comps;
   SingletonsManager* m_services;

public:
   virtual ~ComponentsManager();

   // -------------------------------------------------------------------------
   // Components management
   // -------------------------------------------------------------------------

   /**
    * Adds a new component.
    * 
    * @param component     component we want to add.
    */
   void addComponent(Component<Derived>* component);

   /**
    * Returns the number of registered components.
    *
    * @return           number of registered components
    */
   unsigned int getComponentsCount() const;
   
   /**
    * Returns a component stored under the specified index
    */
   Component<Derived>* getComponent(unsigned int idx);

   // -------------------------------------------------------------------------
   // Services management
   // -------------------------------------------------------------------------
   /**
    * This method registers a new service.
    * 
    * @param T          type of the service
    * @param service    service instance.
    */
   template <typename T>
   void registerService(T& service);

   /**
    * This method queries for a service. If one is not found,
    * an exception will be thrown.
    *
    * @throw std::out_of_range   if the requested service is not found
    * 
    * @return           service instance.
    */
   template <typename T>
   T& requestService();

protected:
   /**
    * Constructor.
    */
   ComponentsManager();
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ComponentsManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_MANAGER_H
