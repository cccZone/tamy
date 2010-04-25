#pragma once

/// @file   ext-RendererView\DirectionalLight.h
/// @brief  graphical representation of a directional light

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct DirectionalLightEntity;
class Light;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of a directional light.
 */
class DirectionalLight : public GraphicalRepresentation
{
private:
   DirectionalLightEntity& m_entity;
   Light* m_light;

public:
   /**
    * Constructor.
    *
    * @param entity     directional light entity which this class instance 
    *                   represents
    */
   DirectionalLight(DirectionalLightEntity& entity);
   ~DirectionalLight();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
