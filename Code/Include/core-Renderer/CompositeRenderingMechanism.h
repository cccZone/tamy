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
private:
   typedef std::map< std::string, int >         MechanismsMap;

private:
   Renderer*                           m_renderer;

   MechanismsMap                       m_mechanismsMap;      
   std::vector< RenderingMechanism* >  m_members;
   std::list< int >                    m_freeSlots;

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
    */
   void add( const std::string& name, RenderingMechanism* mechanism );

   /**
    * Removes a mechanism from the composite.
    *
    * @param name          unique name for the mechanism
    * @param release       should the mechanism be released in the process?
    */
   void remove( const std::string& name, bool release = true );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
