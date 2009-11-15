#include "core-Scene\StaticGeometryEntity.h"
#include "core\Serializer.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(StaticGeometryEntity);

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::StaticGeometryEntity()
: INIT_ENTITY(StaticGeometryEntity)
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::StaticGeometryEntity(const std::vector<MeshDesc*> meshes,
                                           const D3DXMATRIX& situation)
: INIT_ENTITY(StaticGeometryEntity)
, m_meshes(meshes)
, m_situation(situation)
{
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
}

///////////////////////////////////////////////////////////////////////////////
