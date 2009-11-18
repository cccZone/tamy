#include "core-Scene\StaticGeometryEntity.h"
#include "core\Serializer.h"
#include "core\Triangle.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(StaticGeometryEntity);

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::StaticGeometryEntity()
: INIT_ENTITY(StaticGeometryEntity)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::StaticGeometryEntity(const std::vector<MeshDesc*> meshes,
                                           const D3DXMATRIX& situation)
: INIT_ENTITY(StaticGeometryEntity)
, m_meshes(meshes)
, m_situation(situation)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
   calculateGeometry();
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::~StaticGeometryEntity()
{
   unsigned int count = m_meshes.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_meshes[i];
   }
   m_meshes.clear();

   resetGeometry();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryEntity::registerProperties()
{
   PROPERTY("situation", D3DXMATRIX, m_situation);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& StaticGeometryEntity::getBoundingVolume()
{
   m_boundingVol.origin.x = m_situation._41;
   m_boundingVol.origin.y = m_situation._42;
   m_boundingVol.origin.z = m_situation._43;
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& StaticGeometryEntity::getGlobalMtx()
{
   return m_situation;
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& StaticGeometryEntity::getBoundingGeometry()
{
   return m_geometry;
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryEntity::save(Serializer& serializer)
{
   unsigned int count = m_meshes.size();
   serializer.saveInt(count);
   for (unsigned int i = 0; i < count; ++i)
   {
      m_meshes[i]->save(serializer);
   }
   
   serializer.saveMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryEntity::load(Serializer& serializer)
{
   unsigned int count = serializer.loadInt();
   for (unsigned int i = 0; i < count; ++i)
   {
      m_meshes.push_back(new MeshDesc());
      m_meshes.back()->load(serializer);
   }

   serializer.loadMatrix(m_situation);

   resetGeometry();
   calculateGeometry();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryEntity::resetGeometry()
{
   unsigned int count = m_geometry.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_geometry[i];
   }
   m_geometry.clear();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryEntity::calculateGeometry()
{
}

///////////////////////////////////////////////////////////////////////////////
