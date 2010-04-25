#pragma once

/// @file   ext-RendererView\PointLight.h
/// @brief  graphical representation of a point light

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct PointLightEntity;
class Light;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of a point light.
 */
class PointLight : public GraphicalRepresentation
{
private:
   PointLightEntity& m_entity;
   Light* m_light;

public:
   /**
    * Constructor.
    *
    * @param entity     point light entity which this class instance 
    *                   represents
    */
   PointLight(PointLightEntity& entity);
   ~PointLight();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
