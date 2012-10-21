/// @file   core-Renderer\ModelComponent.h
/// @brief  A component that can be attached to a model.
#pragma once

#include "core\Component.h"
#include "core\MemoryRouter.h"


///////////////////////////////////////////////////////////////////////////////

class Model;

///////////////////////////////////////////////////////////////////////////////

/**
 * This component allows to attach a shared component to a model.
 */
template< typename T >
class ModelComponent : public Component<Model>
{
   DECLARE_ALLOCATOR( ModelComponent, AM_DEFAULT );

private:
   T&          m_component;

public:
   /**
    * Constructor.
    *
    * @param camera
    */
   ModelComponent( T& component ) : m_component( component ) {}

   /**
    * Returns a camera instance.
    */
   inline T& get()  {return m_component; }

   // -------------------------------------------------------------------------
   // Component implementation
   // -------------------------------------------------------------------------
   void initialize( Model& mgr ) {}
};

///////////////////////////////////////////////////////////////////////////////
