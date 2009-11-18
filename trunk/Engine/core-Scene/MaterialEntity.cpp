#include "core-Scene\MaterialEntity.h"
#include "core\Serializer.h"


///////////////////////////////////////////////////////////////////////////////

DEFINE_ENTITY(MaterialEntity);

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::MaterialEntity()
: INIT_ENTITY(MaterialEntity)
, m_power(1)
, m_colorOp(MOP_DISABLE)
, m_colorArg1(SC_NONE)
, m_colorArg2(SC_NONE)
, m_alphaOp(MOP_DISABLE)
, m_alphaArg1(SC_NONE)
, m_alphaArg2(SC_NONE)
, m_coordsOp(CC_WRAP)
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialEntity::MaterialEntity(const std::string& matName,
                               const Color& ambient,
                               const Color& diffuse,
                               const Color& specular,
                               const Color& emissive,
                               float power,
                               const std::string& texName,
                               const MatOpCode& colorOp,
                               const SourceCode& colorArg1,
                               const SourceCode& colorArg2,
                               const MatOpCode& alphaOp,
                               const SourceCode& alphaArg1,
                               const SourceCode& alphaArg2,
                               const CoordsOpCode& coordsOp)
: INIT_ENTITY(MaterialEntity)
, m_matName(matName)
, m_ambient(ambient)
, m_diffuse(diffuse)
, m_specular(specular)
, m_emissive(emissive)
, m_power(power)
, m_texName(texName)
, m_colorOp(colorOp)
, m_colorArg1(colorArg1)
, m_colorArg2(colorArg2)
, m_alphaOp(alphaOp)
, m_alphaArg1(alphaArg1)
, m_alphaArg2(alphaArg2)
, m_coordsOp(coordsOp)
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::registerProperties()
{
   PROPERTY("name", std::string, m_matName);
   PROPERTY("ambient color", Color, m_ambient);
   PROPERTY("diffuse color", Color, m_diffuse);
   PROPERTY("specular color", Color, m_specular);
   PROPERTY("emissive color", Color, m_emissive);
   PROPERTY("specular power factor", float, m_power);
   PROPERTY("texture", std::string, m_texName);
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::save(Serializer& serializer)
{
   serializer.saveString(m_matName);
   m_ambient.save(serializer);
   m_diffuse.save(serializer);
   m_specular.save(serializer);
   m_emissive.save(serializer);
   serializer.saveFloat(m_power);
   serializer.saveString(m_texName);
   serializer.saveInt(m_colorOp);
   serializer.saveInt(m_colorArg1);
   serializer.saveInt(m_colorArg2);
   serializer.saveInt(m_alphaOp);
   serializer.saveInt(m_alphaArg1);
   serializer.saveInt(m_alphaArg2);
   serializer.saveInt(m_coordsOp);
}

///////////////////////////////////////////////////////////////////////////////

void MaterialEntity::load(Serializer& serializer)
{
   serializer.loadString(m_matName);
   m_ambient.load(serializer);
   m_diffuse.load(serializer);
   m_specular.load(serializer);
   m_emissive.load(serializer);
   m_power = serializer.loadFloat();
   serializer.loadString(m_texName);
   m_colorOp = (MatOpCode)serializer.loadInt();
   m_colorArg1 = (SourceCode)serializer.loadInt();
   m_colorArg2= (SourceCode)serializer.loadInt();
   m_alphaOp = (MatOpCode)serializer.loadInt();
   m_alphaArg1 = (SourceCode)serializer.loadInt();
   m_alphaArg2 = (SourceCode)serializer.loadInt();
   m_coordsOp = (CoordsOpCode)serializer.loadInt();
}

///////////////////////////////////////////////////////////////////////////////
