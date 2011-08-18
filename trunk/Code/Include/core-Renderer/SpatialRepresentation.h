#pragma once

/// @file   core-Renderer\SpatialRepresentation.h
/// @brief  representation of a renderable from the spatial point of view

#include "core-Renderer\Geometry.h"


///////////////////////////////////////////////////////////////////////////////

class BoundingVolume;
class Renderer;
class StateComparator;

///////////////////////////////////////////////////////////////////////////////

/**
 * Representation of a renderable from the spatial point of view.
 */
class SpatialRepresentation
{
private:
   Geometry&            m_geometry;
   RenderStatesVec      m_renderStates;

   BoundingVolume*      m_globalBoundingVolume;

public:
   /**
    * Constructor.
    *
    * @param geometry               geometry this representation represents
    */
   SpatialRepresentation( Geometry& geometry );
   ~SpatialRepresentation();

   /**
    * Returns the global space bounding volume of the renderable.
    */
   const BoundingVolume& getBoundingVolume();

   /**
    * Returns the stored geometry.
    */
   inline Geometry& getGeometry() { return m_geometry; }

   /**
    * Returns the sorted array of the geometry's render states.
    */
   inline const RenderStatesVec& getRenderStates() { return m_renderStates; }
};

///////////////////////////////////////////////////////////////////////////////
