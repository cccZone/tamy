#include "core-Scene\AnimatedCutsceneEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(AnimatedCutsceneEntity);

///////////////////////////////////////////////////////////////////////////////

AnimatedCutsceneEntity::AnimatedCutsceneEntity()
: INIT_ENTITY(AnimatedCutsceneEntity)
, m_time(0)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
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
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
}

///////////////////////////////////////////////////////////////////////////////

void AnimatedCutsceneEntity::registerProperties()
{
   PROPERTY("m_sceneFileName", "name", std::string, m_sceneFileName);
   PROPERTY("m_animationName", "animation", std::string, m_animationName);
   PROPERTY("m_situation", "situation", D3DXMATRIX, m_situation);
   PROPERTY("m_time", "time", float, m_time);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& AnimatedCutsceneEntity::getBoundingVolume()
{
   m_boundingVol.origin.x = m_situation._41;
   m_boundingVol.origin.y = m_situation._42;
   m_boundingVol.origin.z = m_situation._43;
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& AnimatedCutsceneEntity::getGlobalMtx()
{
   return m_situation;
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& AnimatedCutsceneEntity::getBoundingGeometry()
{
   return m_noGeometry;
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
