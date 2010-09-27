#pragma once

/// @file   core-Renderer\ResourceManagerComponent.h
/// @brief  A component that can be attached to a resource manager.

#include "core\Component.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;

///////////////////////////////////////////////////////////////////////////////

template< typename T >
class ResourceManagerComponent : public Component< ResourcesManager >
{
private:
   T&          m_component;

public:
   /**
    * Constructor.
    *
    * @param camera
    */
   ResourceManagerComponent( T& component ) : m_component( component ) {}

   /**
    * Returns a camera instance.
    */
   inline T& get()  {return m_component; }

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( ResourcesManager& mgr ) {}
};

///////////////////////////////////////////////////////////////////////////////
