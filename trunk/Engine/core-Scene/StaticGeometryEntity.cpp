#include "core-Scene\StaticGeometryEntity.h"
#include "core\Serializer.h"
#include "core\Triangle.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(StaticGeometryEntity);

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::StaticGeometryEntity()
: INIT_ENTITY(StaticGeometryEntity)
, m_localBoundingVol(AABoundingBox())
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryEntity::StaticGeometryEntity(const std::vector<MeshDesc*> meshes,
                                           const D3DXMATRIX& situation)
: INIT_ENTITY(StaticGeometryEntity)
, m_meshes(meshes)
, m_situation(situation)
, m_localBoundingVol(AABoundingBox())
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
   PROPERTY("m_situation", "situation", D3DXMATRIX, m_situation);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& StaticGeometryEntity::getBoundingVolume()
{
   BoundingVolume* bv = m_localBoundingVol * m_situation;
   m_globalBoundingVol = *(dynamic_cast<AABoundingBox*> (bv));
   delete bv;
   return m_globalBoundingVol;
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

   m_localBoundingVol = AABoundingBox();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryEntity::calculateGeometry()
{
   m_localBoundingVol = AABoundingBox(D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX),
                                      D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX));

   unsigned int count = m_meshes.size();
   for (unsigned int meshIdx = 0; meshIdx < count; ++meshIdx)
   {
      const std::vector<LitVertex>& vertices = m_meshes[meshIdx]->m_vertices;
      const std::vector<Face>& faces = m_meshes[meshIdx]->m_faces;

      unsigned int trisCount = faces.size();
      for (unsigned int triIdx = 0; triIdx < trisCount; ++triIdx)
      {
         const D3DXVECTOR3& v1 = vertices[faces[triIdx].idx[0]].m_coords;
         const D3DXVECTOR3& v2 = vertices[faces[triIdx].idx[1]].m_coords;
         const D3DXVECTOR3& v3 = vertices[faces[triIdx].idx[2]].m_coords;

         // create a new triangle
         m_geometry.push_back(new Triangle(v1, v2, v3));

         // calculate geometry bounds
         m_localBoundingVol.modifyUsing(v1);
         m_localBoundingVol.modifyUsing(v2);
         m_localBoundingVol.modifyUsing(v3);
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
