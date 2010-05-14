#ifndef _COMPONENT_H
#define _COMPONENT_H

/// @file   core\Component.h
/// @brief  component that can perform some services

#include <string>
#include <set>

///////////////////////////////////////////////////////////////////////////////

/**
 * A component performs some services with which it enriches the application.
 *
 * Components can operate only in the context of designated components manager.
 * Thus the need to specify the manager type.
 */
template<typename Manager>
class Component
{
private:
   std::string m_name;
   std::set< void* > m_hostedServices;

public:
   virtual ~Component() {}

   /**
    * Returns the name assigned to the component.
    */
   inline const std::string& getComponentName() const { return m_name; }

   /**
    * Components manager will call this method when the component is added to 
    * it in order to perform the component initialization.
    *
    * @param mgr  components manager this component was added to.
    */
   virtual void initialize( Manager& mgr ) {}

   /**
    * Called when a new service is registered in the manager.
    *
    * @param mgr  manager of this component
    */
   virtual void onServiceRegistered( Manager& mgr ) {}

   // -------------------------------------------------------------------------
   // Services management
   // -------------------------------------------------------------------------
   /**
    * Adds an info about a new service the component is hosting.
    *
    * @param service
    */
   void addService( void* service );
   
   /**
    * Removes an info about a service the component is hosting.
    *
    * @param service
    */
   void removeService( void* service );

   /**
    * Checks if the component is hosting a particular service.
    *
    * @param service
    */
   bool isHost( void* service ) const;

protected:
   /**
    * Constructor.
    *
    * @param name    name of the component
    */
   Component( const std::string& name = "") : m_name( name ) {}
};

///////////////////////////////////////////////////////////////////////////////

#include "core\Component.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _COMPONENT_H
