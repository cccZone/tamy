#include "core-Scene\AnimatedCutsceneEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(AnimatedCutsceneEntity);

///////////////////////////////////////////////////////////////////////////////

AnimatedCutsceneEntity::AnimatedCutsceneEntity()
: INIT_ENTITY(AnimatedCutsceneEntity)
, m_time(0)
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

AnimatedCutsceneEntity::AnimatedCutsceneEntity(const std::string& sceneFileName,
                                               const std::string& animationName,
                                               const D3DXMATRIX& situation)
: INIT_ENTITY(AnimatedCutsceneEntity)
, m_sceneFileName(sceneFileName)
, m_animationName(animationName)
, m_situation(situation)
, m_time(0)
{
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutsceneEntity::registerProperties()
{
   PROPERTY("name", std::string, m_sceneFileName);
   PROPERTY("animation", std::string, m_animationName);
   PROPERTY("situation", D3DXMATRIX, m_situation);
   PROPERTY("time", float, m_time);
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutsceneEntity::save(Serializer& serializer)
{
   serializer.saveString(m_sceneFileName);
   serializer.saveString(m_animationName);
   serializer.saveMatrix(m_situation);
   serializer.saveFloat(m_time);
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutsceneEntity::load(Serializer& serializer)
{
   serializer.loadString(m_sceneFileName);
   serializer.loadString(m_animationName);
   serializer.loadMatrix(m_situation);
   m_time = serializer.loadFloat();
}

///////////////////////////////////////////////////////////////////////////////
