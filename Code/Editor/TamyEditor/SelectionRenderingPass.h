#pragma once

/// @file   TamyEditor\SelectionRenderingPass.h
/// @brief  a rendering pass dedicated to entities selection mechanism

#include "core.h"
#include "core-Renderer/RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

class Entity;
class SelectedEntityRepresentation;

///////////////////////////////////////////////////////////////////////////////

/**
 * A rendering pass dedicated to entities selection mechanism.
 */
class SelectionRenderingPass : public RenderingMechanism, public GenericFactory< Entity, SelectedEntityRepresentation >
{
private:
   SelectedEntityRepresentation*       m_selectedRepresentation;

public:
   /**
    * Constructor.
    */
   SelectionRenderingPass();
   ~SelectionRenderingPass();

   /**
    * Puts a new entity up for visualization.
    *
    * @param entity
    */
   void add( Entity& entity );

   /**
    * Removes an entity from being visualized as selected.
    *
    * @param entity
    */
   void remove( Entity& entity );

   // -------------------------------------------------------------------------
   // RenderingMechanism implementation
   // -------------------------------------------------------------------------
   void initialize( Renderer& renderer );
   void deinitialize( Renderer& renderer );
   void render( Renderer& renderer );
};

///////////////////////////////////////////////////////////////////////////////
