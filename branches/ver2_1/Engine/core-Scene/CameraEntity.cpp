#include "core-Scene\CameraEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(CameraEntity);

///////////////////////////////////////////////////////////////////////////////

CameraEntity::CameraEntity()
: INIT_ENTITY(CameraEntity)
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

CameraEntity::CameraEntity(const std::string& name,
                           const D3DXMATRIX& situation)
: INIT_ENTITY(CameraEntity)
, m_name(name)
, m_situation(situation)
{
}

///////////////////////////////////////////////////////////////////////////////

void CameraEntity::registerProperties()
{
   PROPERTY("name", std::string, m_name);
   PROPERTY("situation", D3DXMATRIX, m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void CameraEntity::save(Serializer& serializer)
{
   serializer.saveString(m_name);
   serializer.saveMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void CameraEntity::load(Serializer& serializer)
{
   serializer.loadString(m_name);
   serializer.loadMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////
