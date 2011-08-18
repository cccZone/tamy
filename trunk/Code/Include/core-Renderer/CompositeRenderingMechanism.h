#pragma once

#include "core-Renderer\RenderingMechanism.h"
#include <vector>
#include <map>
#include <string>


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

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
