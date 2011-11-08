#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include <vector>
#include <map>
#include <string>
#include <list>


///////////////////////////////////////////////////////////////////////////////

/**
 * A composite that can execute many rendering mechanisms in a sequential
 * order.
 */
class CompositeRenderingMechanism : public RenderingMechanism
{
   struct MechDef
   {
      std::string                               m_name;
      RenderingMechanism*                       m_mechanism;
      bool                                      m_release;

      MechDef( const std::string& name, RenderingMechanism* mechanism, bool release )
         : m_name( name )
         , m_mechanism( mechanism )
         , m_release( release )
      {
      }

      ~MechDef()
      {
         if ( m_release )
         {
            delete m_mechanism;
            m_mechanism = NULL;
         }
      }
   };
private:
   Renderer*                                    m_renderer;
   std::vector< MechDef* >                      m_members;

public:
   /**
    * Constructor.
    */
   CompositeRenderingMechanism();
   ~CompositeRenderingMechanism();

   /**
    * Adds a new mechanism to the composite, or replaces one that is registered
    * under the specified name.
    *
    * @param name          unique name for the mechanism
    * @param mechanism     rendering mechanism we want to run in the composite
    * @param manage        should the composite manage the mechanism's lifetime
    */
   void add( const std::string& name, RenderingMechanism* mechanism, bool manage = true );

   /**
    * Removes a mechanism from the composite.
    *
    * @param name          unique name for the mechanism
    */
   void remove( const std::string& name );

   /**
    * Removes all added mechanisms, effectively cleaning the composite.
    */
   void reset();

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
