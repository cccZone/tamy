#pragma once

/// @file   core-Renderer\DefaultAttributeSorter.h
/// @brief  non-sorting renderables container

#include "core-Renderer\AttributeSorter.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

/**
 * This sorter implementation aggregates all added renderables
 * in the exact order they were added in and doesn't perform
 * any additional sorting.
 */
class DefaultAttributeSorter : public AttributeSorter
{
private:
   std::vector<Attributed*> m_objects;

public:
   // -------------------------------------------------------------------------
   // AttributeSorter implementation
   // -------------------------------------------------------------------------
   void add(Attributed& object);

   void remove(Attributed& object);

   void render();
};

///////////////////////////////////////////////////////////////////////////////

