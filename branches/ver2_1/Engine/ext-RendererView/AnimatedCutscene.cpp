#include "ext-RendererView\AnimatedCutscene.h"
#include "ext-RendererView\RendererView.h"
#include "core-Scene\AnimatedCutsceneEntity.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Skeleton.h"
#include "core-ResourceManagement\GraphicalEntityLoader.h"
#include "core-Renderer\RenderableNode.h"
#include "tamy\Tamy.h"
#include <set>


namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

AnimatedCutscene::AnimatedCutscene(AnimatedCutsceneEntity& entity)
: m_entity(entity)
, m_entityInstance(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

AnimatedCutscene::~AnimatedCutscene()
{
   m_entityInstance = NULL;
   delete m_animation; m_animation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutscene::initialize(RendererView& rendererView)
{
   // get a dynamic graphical entity
   AbstractGraphicalEntity* entity = NULL;
   if (rendererView.dynamicEntities().is(m_entity.m_sceneFileName))
   {
      entity = &(rendererView.dynamicEntities().get(m_entity.m_sceneFileName));
   }
   else
   {
      GraphicalEntityLoader entityLoader(rendererView.factory(), rendererView.materials());

      entity = entityLoader.load(m_entity.m_sceneFileName, TAMY.meshLoaders());
      rendererView.dynamicEntities().add(entity);
   }

   /// instantiate the graphical entity
   m_entityInstance = new GraphicalEntityInstantiator(m_entity.m_sceneFileName);
   m_entityInstance->attachEntity(*entity);
   m_entityInstance->setLocalMtx(m_entity.m_situation);

   // add the instance to the geometry management system
   rendererView.dynamicGeometry().addNode(m_entityInstance);

   // create the skeleton for the entity and set up a default animation,
   // if there's one
   m_animation = entity->instantiateSkeleton(*m_entityInstance);

   std::set<std::string> animNames;
   m_animation->getAnimationNames(animNames);

   bool requestedAnimExists = (animNames.find(m_entity.m_animationName) != animNames.end());
   if (requestedAnimExists)
   {
      // activate an animation only if it exists
      m_animation->activateAnimation(m_entity.m_animationName, true);
      m_animation->setPosition(m_entity.m_animationName, m_entity.m_time);
   }
   else
   {
      delete m_animation;
      m_animation = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutscene::deinitialize(RendererView& rendererView)
{
   rendererView.dynamicGeometry().removeNode(*m_entityInstance);
   delete m_entityInstance; m_entityInstance = NULL;
   delete m_animation; m_animation = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutscene::update(float timeElapsed)
{
   if (m_animation == NULL) {return;}
   // TODO:
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
