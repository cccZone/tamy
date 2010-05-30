#pragma once

/// @file   TamyEditor/SelectedEntityRepresentation.h
/// @brief  a visual representation of a selected entity

#include "core-Renderer\AttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

class SelectionManager;

///////////////////////////////////////////////////////////////////////////////

/**
 * A visual representation of a selected entity.
 */
class SelectedEntityRepresentation : public Attributed
{
public:
   virtual ~SelectedEntityRepresentation() {}

   /**
    * Initializes the representation once it's created by the host.
    *
    * @param host    host that created this representation
    */
   virtual void initialize( SelectionManager& host ) = 0;

   /**
    * Renders the representation.
    */
   virtual void render() = 0;
};

///////////////////////////////////////////////////////////////////////////////
