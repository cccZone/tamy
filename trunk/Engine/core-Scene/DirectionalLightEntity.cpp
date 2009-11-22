#include "core-Scene\DirectionalLightEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(DirectionalLightEntity);

///////////////////////////////////////////////////////////////////////////////

DirectionalLightEntity::DirectionalLightEntity()
: INIT_ENTITY(DirectionalLightEntity)
{
   D3DXMatrixIdentity(&m_situation);
}

///////////////////////////////////////////////////////////////////////////////

void DirectionalLightEntity::registerProperties()
{
   PROPERTY("name", std::string, m_name);
   PROPERTY("situation", D3DXMATRIX, m_situation);
   PROPERTY("ambient", Color, m_ambient);
   PROPERTY("diffuse", Color, m_diffuse);
   PROPERTY("specular", Color, m_specular);
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
{
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
