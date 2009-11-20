#include "core-Scene\CameraEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(CameraEntity);

///////////////////////////////////////////////////////////////////////////////

CameraEntity::CameraEntity()
: INIT_ENTITY(CameraEntity)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

CameraEntity::CameraEntity(const std::string& name,
                           const D3DXMATRIX& situation)
: INIT_ENTITY(CameraEntity)
, m_name(name)
, m_situation(situation)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
}

///////////////////////////////////////////////////////////////////////////////

void CameraEntity::registerProperties()
{
   PROPERTY("name", std::string, m_name);
   PROPERTY("situation", D3DXMATRIX, m_situation);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& CameraEntity::getBoundingVolume()
{
   m_boundingVol.origin.x = m_situation._41;
   m_boundingVol.origin.y = m_situation._42;
   m_boundingVol.origin.z = m_situation._43;
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& CameraEntity::getGlobalMtx()
{
   return m_situation;
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& CameraEntity::getBoundingGeometry()
{
   return m_noGeometry;
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
