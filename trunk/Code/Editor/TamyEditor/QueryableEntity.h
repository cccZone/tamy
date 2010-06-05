#pragma once

/// @file   TamyEngine\QueryableEntity.h
/// @brief  An entity representation which can be picked of the screen 
///         by pointing at it

#include "core-Renderer\AttributeSorter.h"


///////////////////////////////////////////////////////////////////////////////

class SceneQueryEffect;

///////////////////////////////////////////////////////////////////////////////

/**
 * An entity representation which can be picked of the screen by pointing at it.
 */
class QueryableEntity : public Attributed
{
public:
   virtual ~QueryableEntity() {}

   /**
    * Initializes the representation.
    *
    * @param effect
    */
   virtual void initialize( SceneQueryEffect& effect ) = 0;

};

///////////////////////////////////////////////////////////////////////////////
