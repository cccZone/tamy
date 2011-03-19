#ifndef _COMPONENTS_MANAGER_H
#define _COMPONENTS_MANAGER_H

/// @file   core\ComponentsManager.h
/// @brief  base for all component oriented classes

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class SingletonsManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * Such a manager manages the services and components.
 *
 * A service features only one instance of a class being able to provide some
 * functionality - we can't have many instances of the same class registered 
 * as services simultaneously.
 * A service can be queried by that very class type.
 *
 * On the other hand we have components - each component type can have many 
 * instances registered with the same manager. Each component has a name,
 * so components can be queried using that name or by their type.
 */
template < typename Derived >
class ComponentsManager
{
public:
   typedef std::vector< Component< Derived >* >   ComponentsArr;

private:
   ComponentsArr                          m_comps;
   SingletonsManager*                     m_services;

public:
   virtual ~ComponentsManager();

    /**
    * Updates the state of the components.
    *
    * @param timeElapsed      time elapsed since the last frame
    */
   virtual void update( float timeElapsed );

   // -------------------------------------------------------------------------
   // Components management
   // -------------------------------------------------------------------------

   /**
    * Adds a new component.
    * 
    * @param component     component we want to add.
    */
   void addComponent( Component< Derived >* component );

   /**
    * Removes the selected component.
    *
    * @param component
    */
   void removeComponent( Component< Derived >& component );

   /**
    * Returns the number of registered components.
    *
    * @return           number of registered components
    */
   unsigned int getComponentsCount() const;
   
   /**
    * Returns a component stored under the specified index
    */
   Component< Derived >* getComponent( unsigned int idx );

   /**
    * Finds all registered components with the specified name.
    *
    * @param name             name of the components we want to find
    * @param outComponents    found components will be returned through this collection
    */
   void findComponents( const std::string& name, ComponentsArr& outComponents ) const;

   /**
    * Finds all registered components with the specified type.
    *
    * @param T                type of the component
    * @param outComponents    found components will be returned through this collection
    */
   template< typename T >
   void findComponents( std::vector< T* >& outComponents ) const;

   // -------------------------------------------------------------------------
   // Services management
   // -------------------------------------------------------------------------
   /**
    * This method registers a new service.
    * 
    * @param T          type of the service
    * @param service    service instance.
    */
   template < typename T >
   void registerService( Component< Derived >& host, T& service );

   /**
    * This method removes a service of the specified type.
    * 
    * @param T          type of the service
    */
   template < typename T >
   void removeService( Component< Derived >& host );

   /**
    * Tells whether a service of the specified type is registered.
    * 
    * @param T          type of the service
    */
   template < typename T >
   bool hasService() const;

   /**
    * Tells whether the specified service instance matches the currently registered one.
    * 
    * @param T          type of the service
    * @param service    instance of the service we want to check
    */
   template < typename T >
   bool needsUpdate( T& service ) const;

   /**
    * This method queries for a service. If one is not found,
    * an exception will be thrown.
    *
    * @throw std::out_of_range   if the requested service is not found
    * 
    * @return           service instance.
    */
   template < typename T >
   T& requestService();

protected:
   /**
    * Constructor.
    */
   ComponentsManager();

   // -------------------------------------------------------------------------
   // Notifications
   // -------------------------------------------------------------------------

   /**
    * Called when a component is added to the manager.
    *
    * @param component
    */
   virtual void onComponentAdded( Component< Derived >& component ) {}

   /**
    * Called when a component is about to be removed from the manager.
    *
    * @param component
    */
   virtual void onComponentRemoved( Component< Derived >& component ) {}

private:
   void notifyAboutService( void* service );
};

///////////////////////////////////////////////////////////////////////////////

#include "core\ComponentsManager.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _COMPONENTS_MANAGER_H
