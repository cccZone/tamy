#pragma once

/// @file   core-Renderer\RenderableRepresentation.h
/// @brief  representation of a renderable entity that can be rendered

#include "core-Renderer\AttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingView;

///////////////////////////////////////////////////////////////////////////////

/**
 * This representation of a renderable entity works with a RandererView
 * and allows to render such an entity.
 */
class RenderableRepresentation : public Attributed
{
private:
   Renderable&       m_renderable;
   RenderingView&    m_view;

public:
   /**
    * Constructor.
    *
    * @param renderable    entity this instance represents
    * @param view          view that manages this instance
    */
   RenderableRepresentation( Renderable& renderable, RenderingView& view );

   // -------------------------------------------------------------------------
   // Attributed implementation
   // -------------------------------------------------------------------------
   const Attributes& getAttributes() const;

   void render();
};

///////////////////////////////////////////////////////////////////////////////
