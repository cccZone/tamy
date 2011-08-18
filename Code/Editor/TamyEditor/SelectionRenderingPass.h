#pragma once

/// @file   TamyEditor\SelectionRenderingPass.h
/// @brief  a rendering pass dedicated to entities selection mechanism

#include "core.h"
#include "core-Renderer/RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class SelectedEntityRepresentation;
class SelectionManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pass dedicated to entities selection mechanism.
 */
class SelectionRenderingPass : public RenderingMechanism, public GenericFactory< Entity, SelectedEntityRepresentation >
{
private:
   SelectionManager&                   m_host;
   SelectedEntityRepresentation*       m_selectedRepresentation;

public:
   /**
    * Constructor.
    *
    * @param host          parent selection manager
    */
   SelectionRenderingPass( SelectionManager& host );
   ~SelectionRenderingPass();

   /**
    * Puts a new entity up for visualization ( or just erases the old one 
    * if NULL is specified ).
    *
    * @param entity
    */
   void set( Entity* entity );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
