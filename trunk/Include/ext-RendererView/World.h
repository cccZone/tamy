#pragma once

/// @file   ext-RendererView\World.h
/// @brief  graphical representation of the world.

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct WorldEntity;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of the world - all the data storages
 * required to contain it etc.
 */
class World : public GraphicalRepresentation
{
private:
   WorldEntity& m_entity;

public:
   /**
    * Constructor.
    * 
    * @param entity     sky box entity this representation is for
    */
   World(WorldEntity& entity);
   ~World();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
