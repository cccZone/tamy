#include "core-Scene\PointLightEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(PointLightEntity);

///////////////////////////////////////////////////////////////////////////////

PointLightEntity::PointLightEntity()
: INIT_ENTITY(PointLightEntity)
, m_range(0)
, m_constAtt(0)
, m_linearAtt(0)
, m_quadAtt(0)
, m_situation(0, 0, 0)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
}

///////////////////////////////////////////////////////////////////////////////

PointLightEntity::PointLightEntity(const std::string& name,
                                   const Color& ambient, 
                                   const Color& diffuse, 
                                   const Color& specular,
                                   float range,
                                   float constAtt, float linearAtt, float quadAtt,
                                   const D3DXVECTOR3& situation)
: INIT_ENTITY(PointLightEntity)
, m_name(name)
, m_ambient(ambient)
, m_diffuse(diffuse)
, m_specular(specular)
, m_range(range)
, m_constAtt(constAtt)
, m_linearAtt(linearAtt)
, m_quadAtt(quadAtt)
, m_situation(situation)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
}

///////////////////////////////////////////////////////////////////////////////

void PointLightEntity::registerProperties()
{
   PROPERTY("name", std::string, m_name);
   PROPERTY("position", D3DXVECTOR3, m_situation);
   PROPERTY("ambient color", Color, m_ambient);
   PROPERTY("diffuse color", Color, m_diffuse);
   PROPERTY("specular color", Color, m_specular);
   PROPERTY("range", float, m_range);
   PROPERTY("attenuation: constant", float, m_constAtt);
   PROPERTY("attenuation: linear", float, m_linearAtt);
   PROPERTY("attenuation: quadratic", float, m_quadAtt);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& PointLightEntity::getBoundingVolume()
{
   m_boundingVol.origin = m_situation;
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& PointLightEntity::getGlobalMtx()
{
   D3DXMatrixTranslation(&m_situationMtx, 
                         m_situation.x, 
                         m_situation.y, 
                         m_situation.z);
   return m_situationMtx;
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& PointLightEntity::getBoundingGeometry()
{
   return m_noGeometry;
}

///////////////////////////////////////////////////////////////////////////////

void PointLightEntity::save(Serializer& serializer)
{
   serializer.saveString(m_name);

   m_ambient.save(serializer);
   m_diffuse.save(serializer);
   m_specular.save(serializer);

   serializer.saveFloat(m_range);
   serializer.saveFloat(m_constAtt);
   serializer.saveFloat(m_linearAtt);
   serializer.saveFloat(m_quadAtt);

   serializer.saveVec3(m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void PointLightEntity::load(Serializer& serializer)
{
   serializer.loadString(m_name);

   m_ambient.load(serializer);
   m_diffuse.load(serializer);
   m_specular.load(serializer);

   m_range = serializer.loadFloat();
   m_constAtt = serializer.loadFloat();
   m_linearAtt = serializer.loadFloat();
   m_quadAtt = serializer.loadFloat();

   serializer.loadVec3(m_situation);
}

///////////////////////////////////////////////////////////////////////////////
