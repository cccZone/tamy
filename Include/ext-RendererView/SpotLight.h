#pragma once

/// @file   ext-RendererView\SpotLight.h
/// @brief  graphical representation of a spot light

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct SpotLightEntity;
class Light;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of a spot light.
 */
class SpotLight : public GraphicalRepresentation
{
private:
   SpotLightEntity& m_entity;
   Light* m_light;

public:
   /**
    * Constructor.
    *
    * @param entity     spot light entity which this class instance 
    *                   represents
    */
   SpotLight(SpotLightEntity& entity);
   ~SpotLight();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
