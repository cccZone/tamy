#pragma once

/// @file   TamyEditor/SelectedEntityRepresentation.h
/// @brief  a visual representation of a selected entity


///////////////////////////////////////////////////////////////////////////////

class Renderer;

///////////////////////////////////////////////////////////////////////////////

/**
 * A visual representation of a selected entity.
 */
class SelectedEntityRepresentation
{
public:
   virtual ~SelectedEntityRepresentation() {}

   /**
    * Renders the representation.
    *
    * @param renderer
    */
   virtual void render( Renderer& renderer ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
