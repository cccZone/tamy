#pragma once

/// @file   ext-RendererView\GraphicalRepresentation.h
/// @brief  graphical representation of an entity

#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{


///////////////////////////////////////////////////////////////////////////////

class RendererView;

///////////////////////////////////////////////////////////////////////////////

/**
 * This interface specifies operations a renderer view will perform 
 * on a graphical representation of a model entity.
 */
class GraphicalRepresentation
{
public:
   virtual ~GraphicalRepresentation() {}

   /**
    * This method allows to initialize a newly created representation.
    *
    * @param rendererView     a renderer view this representation is
    *                         created in.
    */
   virtual void initialize(RendererView& rendererView) = 0;

   /**
    * This method allows to deinitialize a representation that's
    * about to be removed from a view.
    *
    * @param rendererView     a renderer view this representation is
    *                         created in.
    */
   virtual void deinitialize(RendererView& rendererView) = 0;

   /**
    * This method updates the state of the representation.
    *
    * @param timeElapsed   amount of time that has elapsed since the last time
    *                      this method was called
    */
   virtual void update(float timeElapsed) = 0;
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
