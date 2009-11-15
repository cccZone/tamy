#pragma once

/// @file   ext-RendererView\StaticGeometry.h
/// @brief  graphical representation of static geometry

#include "ext-RendererView\GraphicalRepresentation.h"
#include "core\StaticGeometryOctree.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

struct StaticGeometryEntity;
class GraphicalEntitiesFactory;
class StaticGeometryRenderable;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of static geometry.
 */
class StaticGeometry : public GraphicalRepresentation
{
private:
   StaticGeometryEntity& m_entity;
   std::vector<SGHandle> m_geometry;

public:
   /**
    * Constructor.
    *
    * @param entity     static geometry entity this representation is for
    */
   StaticGeometry(StaticGeometryEntity& entity);
   ~StaticGeometry();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
