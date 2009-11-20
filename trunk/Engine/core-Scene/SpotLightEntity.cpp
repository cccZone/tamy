#include "core-Scene\SpotLightEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(SpotLightEntity);

///////////////////////////////////////////////////////////////////////////////

SpotLightEntity::SpotLightEntity()
: INIT_ENTITY(SpotLightEntity)
, m_range(0)
, m_constAtt(0)
, m_linearAtt(0)
, m_quadAtt(0)
, m_fallOff(0)
, m_theta(0)
, m_phi(0)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

SpotLightEntity::SpotLightEntity(const std::string& name,
                                 const Color& ambient, 
                                 const Color& diffuse, 
                                 const Color& specular,
                                 float range,
                                 float constAtt, float linearAtt, float quadAtt,
                                 float fallOff, float theta, float phi,
                                 const D3DXMATRIX& situation)
: INIT_ENTITY(SpotLightEntity)
, m_name(name)
, m_ambient(ambient)
, m_diffuse(diffuse)
, m_specular(specular)
, m_range(range)
, m_constAtt(constAtt)
, m_linearAtt(linearAtt)
, m_quadAtt(quadAtt)
, m_fallOff(fallOff)
, m_theta(theta)
, m_phi(phi)
, m_situation(situation)
, m_boundingVol(BoundingSphere(D3DXVECTOR3(0, 0, 0), 1))
{
}

///////////////////////////////////////////////////////////////////////////////

void SpotLightEntity::registerProperties()
{
   PROPERTY("m_name", "name", std::string, m_name);
   PROPERTY("m_situation", "situation", D3DXMATRIX, m_situation);
   PROPERTY("m_ambient", "ambient", Color, m_ambient);
   PROPERTY("m_diffuse", "diffuse", Color, m_diffuse);
   PROPERTY("m_specular", "specular", Color, m_specular);
   PROPERTY("m_range", "range", float, m_range);
   PROPERTY("m_constAtt", "attenuation: constant", float, m_constAtt);
   PROPERTY("m_linearAtt", "attenuation: linear", float, m_linearAtt);
   PROPERTY("m_quadAtt", "attenuation: quadratic", float, m_quadAtt);
   PROPERTY("m_constAtt", "spot cone: fall off", float, m_constAtt);
   PROPERTY("m_linearAtt", "spot cone: theta", float, m_linearAtt);
   PROPERTY("m_quadAtt", "spot cone: phi", float, m_quadAtt);
}

///////////////////////////////////////////////////////////////////////////////

const BoundingVolume& SpotLightEntity::getBoundingVolume()
{
   m_boundingVol.origin.x = m_situation._41;
   m_boundingVol.origin.y = m_situation._42;
   m_boundingVol.origin.z = m_situation._43;
   return m_boundingVol;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& SpotLightEntity::getGlobalMtx()
{
   return m_situation;
}

///////////////////////////////////////////////////////////////////////////////

const Array<Triangle*>& SpotLightEntity::getBoundingGeometry()
{
   return m_noGeometry;
}

///////////////////////////////////////////////////////////////////////////////

void SpotLightEntity::save(Serializer& serializer)
{
   serializer.saveString(m_name);

   m_ambient.save(serializer);
   m_diffuse.save(serializer);
   m_specular.save(serializer);

   serializer.saveFloat(m_range);
   serializer.saveFloat(m_constAtt);
   serializer.saveFloat(m_linearAtt);
   serializer.saveFloat(m_quadAtt);

   serializer.saveFloat(m_fallOff);
   serializer.saveFloat(m_theta);
   serializer.saveFloat(m_phi);

   serializer.saveMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void SpotLightEntity::load(Serializer& serializer)
{
   serializer.loadString(m_name);

   m_ambient.load(serializer);
   m_diffuse.load(serializer);
   m_specular.load(serializer);

   m_range = serializer.loadFloat();
   m_constAtt = serializer.loadFloat();
   m_linearAtt = serializer.loadFloat();
   m_quadAtt = serializer.loadFloat();

   m_fallOff = serializer.loadFloat();
   m_theta = serializer.loadFloat();
   m_phi = serializer.loadFloat();

   serializer.loadMatrix(m_situation);
}

///////////////////////////////////////////////////////////////////////////////
