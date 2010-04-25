#pragma once

/// @file   core\Component.h
/// @brief  component that can perform some services


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
public:
   virtual ~Component() {}

   /**
    * Components manager will call this method when the component is added to 
    * it in order to perform the component initialization.
    *
    * @param mgr  components manager this component was added to.
    */
   virtual void initialize(Manager& mgr) = 0;
};

///////////////////////////////////////////////////////////////////////////////
