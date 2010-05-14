#pragma once

/// @file   TamyEditor\Grid.h
/// @brief  a graphical widget displaying an infinite grid

#include "core-MVC\Entity.h"


///////////////////////////////////////////////////////////////////////////////

class Renderer;
class ResourcesManager;
class LineSegments;

///////////////////////////////////////////////////////////////////////////////

/**
 * An infinite grid mesh.
 */
class Grid : public Entity
{
   DECLARE_CLASS(Grid)

public:
   /**
    * Default constructor.
    */
   Grid();

   /**
    * Constructor.
    *
    * @param renderer         renderer used to display the component
    */
   Grid(Renderer& renderer, ResourcesManager& rm);
};

///////////////////////////////////////////////////////////////////////////////
