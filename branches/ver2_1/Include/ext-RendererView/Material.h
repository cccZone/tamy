#pragma once

/// @file   ext-RendererView\Material.h
/// @brief  graphical representation of a material.

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct MaterialEntity;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of a sky box scenery.
 */
class Material : public GraphicalRepresentation
{
private:
   MaterialEntity& m_entity;

public:
   /**
    * Constructor.
    * 
    * @param entity     material entity this representation is for
    */
   Material(MaterialEntity& entity);
   ~Material();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
