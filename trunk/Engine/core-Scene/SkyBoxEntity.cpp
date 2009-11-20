#include "core-Scene\SkyBoxEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(SkyBoxEntity);

///////////////////////////////////////////////////////////////////////////////

SkyBoxEntity::SkyBoxEntity()
: INIT_ENTITY(SkyBoxEntity)
{
}

///////////////////////////////////////////////////////////////////////////////

SkyBoxEntity::SkyBoxEntity(const std::string& frontTex,
                           const std::string& backTex,
                           const std::string& leftTex,
                           const std::string& rightTex,
                           const std::string& topTex,
                           const std::string& bottomTex)
: INIT_ENTITY(SkyBoxEntity)
, m_frontTex(frontTex)
, m_backTex(backTex)
, m_leftTex(leftTex)
, m_rightTex(rightTex)
, m_topTex(topTex)
, m_bottomTex(bottomTex)
{
}

///////////////////////////////////////////////////////////////////////////////

void SkyBoxEntity::registerProperties()
{
   PROPERTY("m_frontTex", "front", std::string, m_frontTex);
   PROPERTY("m_backTex", "back", std::string, m_backTex);
   PROPERTY("m_leftTex", "left", std::string, m_leftTex);
   PROPERTY("m_rightTex", "right", std::string, m_rightTex);
   PROPERTY("m_topTex", "top", std::string, m_topTex);
   PROPERTY("m_bottomTex", "bottom", std::string, m_bottomTex);
}

///////////////////////////////////////////////////////////////////////////////

void SkyBoxEntity::save(Serializer& serializer)
{
   serializer.saveString(m_frontTex);
   serializer.saveString(m_backTex);
   serializer.saveString(m_leftTex);
   serializer.saveString(m_rightTex);
   serializer.saveString(m_topTex);
   serializer.saveString(m_bottomTex);
}

///////////////////////////////////////////////////////////////////////////////

void SkyBoxEntity::load(Serializer& serializer)
{
   serializer.loadString(m_frontTex);
   serializer.loadString(m_backTex);
   serializer.loadString(m_leftTex);
   serializer.loadString(m_rightTex);
   serializer.loadString(m_topTex);
   serializer.loadString(m_bottomTex);
}

///////////////////////////////////////////////////////////////////////////////
