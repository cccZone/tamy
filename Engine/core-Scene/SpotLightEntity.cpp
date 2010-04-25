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
{
}

///////////////////////////////////////////////////////////////////////////////

void SpotLightEntity::registerProperties()
{
   PROPERTY("name", std::string, m_name);
   PROPERTY("situation", D3DXMATRIX, m_situation);
   PROPERTY("ambient", Color, m_ambient);
   PROPERTY("diffuse", Color, m_diffuse);
   PROPERTY("specular", Color, m_specular);
   PROPERTY("range", float, m_range);
   PROPERTY("attenuation: constant", float, m_constAtt);
   PROPERTY("attenuation: linear", float, m_linearAtt);
   PROPERTY("attenuation: quadratic", float, m_quadAtt);
   PROPERTY("spot cone: fall off", float, m_constAtt);
   PROPERTY("spot cone: theta", float, m_linearAtt);
   PROPERTY("spot cone: phi", float, m_quadAtt);
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
