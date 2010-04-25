#pragma once

/// @file   core-Renderer\SpatialRepresentation.h
/// @brief  representation of a renderable from the spatial point of view


///////////////////////////////////////////////////////////////////////////////

class Renderable;
class BoundingVolume;

///////////////////////////////////////////////////////////////////////////////

/**
 * Representation of a renderable from the spatial point of view.
 */
class SpatialRepresentation
{
private:
   Renderable& m_renderable;

   BoundingVolume* m_globalBoundingVolume;
   int m_visibilityTag;

public:
   /**
    * Constructor.
    *
    * @param renderable    renderable this representation represents
    */
   SpatialRepresentation(Renderable& renderable);
   ~SpatialRepresentation();

   /**
    * Returns the global space bounding volume of the renderable.
    */
   const BoundingVolume& getBoundingVolume();

   /**
    * This method assigns a new visibility tag to the renderable.
    *
    * @param tag  new visibility tag
    */
   void tagAsVisible(int tag);

   /**
    * This method evaluates if the renderable is visible or not.
    * If it's current visibility tag matches the value of the reference
    * tag specified as the method parameter, the renderable is considered
    * visible.
    *
    * @param referenceVisiblilityTag
    */
   void setVisible(int referenceVisiblilityTag);
};

///////////////////////////////////////////////////////////////////////////////
