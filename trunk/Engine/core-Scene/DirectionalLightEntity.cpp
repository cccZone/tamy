#include "core-Scene\DirectionalLightEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(DirectionalLightEntity);

///////////////////////////////////////////////////////////////////////////////

DirectionalLightEntity::DirectionalLightEntity()
: INIT_ENTITY(DirectionalLightEntity)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLightEntity::registerProperties()
{
   PROPERTY("m_name", "name", std::string, m_name);
   PROPERTY("m_situation", "situation", D3DXMATRIX, m_situation);
   PROPERTY("m_ambient", "ambient", Color, m_ambient);
   PROPERTY("m_diffuse", "diffuse", Color, m_diffuse);
   PROPERTY("m_specular", "specular", Color, m_specular);
}

///////////////////////////////////////////////////////////////////////////////

DirectionalLightEntity::DirectionalLightEntity(const std::string& name,
                                               const Color& ambient, 
                                               const Color& diffuse, 
                                               const Color& specular,
                                               const D3DXMATRIX& situation)
: INIT_ENTITY(DirectionalLightEntity)
, m_name(name)
, m_ambient(ambient)
, m_diffuse(diffuse)
, m_specular(specular)
, m_situation(situation)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& DirectionalLightEntity::getBoundingVolume()
{
   m_boundingVol.origin.x = m_situation._41;
   m_boundingVol.origin.y = m_situation._42;
   m_boundingVol.origin.z = m_situation._43;
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& DirectionalLightEntity::getGlobalMtx()
{
   return m_situation;
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& DirectionalLightEntity::getBoundingGeometry()
{
   return m_noGeometry;
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLightEntity::save(Serializer& serializer)
{
   serializer.saveString(m_name);

   m_ambient.save(serializer);
   m_diffuse.save(serializer);
   m_specular.save(serializer);

   serializer.saveMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLightEntity::load(Serializer& serializer)
{
   serializer.loadString(m_name);

   m_ambient.load(serializer);
   m_diffuse.load(serializer);
   m_specular.load(serializer);

   serializer.loadMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////
