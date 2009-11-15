#pragma once

/// @file   ext-RendererView\AnimatedCutscene.h
/// @brief  graphical representation of an animated cutscene.

#include "ext-RendererView\GraphicalRepresentation.h"


///////////////////////////////////////////////////////////////////////////////

struct AnimatedCutsceneEntity;
class GraphicalEntityInstantiator;
class Skeleton;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

/**
 * This is a graphical representation of an animated cutscene.
 */
class AnimatedCutscene : public GraphicalRepresentation
{
private:
   AnimatedCutsceneEntity& m_entity;
   GraphicalEntityInstantiator* m_entityInstance;
   Skeleton* m_animation;

public:
   /**
    * Constructor.
    * 
    * @param entity     animated cutscene entity this representation is for
    */
   AnimatedCutscene(AnimatedCutsceneEntity& entity);
   ~AnimatedCutscene();

   // -------------------------------------------------------------------------
   // GraphicalRepresentation implementation
   // -------------------------------------------------------------------------
   void initialize(RendererView& rendererView);

   void deinitialize(RendererView& rendererView);

   void update(float timeElapsed);
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
