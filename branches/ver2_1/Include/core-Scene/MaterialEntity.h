#pragma once

/// @file   core-Scene\MaterialEntity.h
/// @brief  description of a material

#include "core-Scene\Entity.h"
#include "core\Color.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

/**
 * This class describes a material used to render meshes.
 */
struct MaterialEntity : public Entity
{
   std::string m_matName;

   Color m_ambient;
   Color m_diffuse;
   Color m_specular;
   Color m_emissive;
   float m_power;

   std::string m_texName;
   MatOpCode m_colorOp;
   SourceCode m_colorArg1;
   SourceCode m_colorArg2;
   MatOpCode m_alphaOp;
   SourceCode m_alphaArg1;
   SourceCode m_alphaArg2;
   CoordsOpCode m_coordsOp;

   /**
    * Default constructor.
    */
   MaterialEntity();

   /**
    * Constructor.
    */
   MaterialEntity(const std::string& matName,
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
                const CoordsOpCode& coordsOp);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
   void save(Serializer& serializer);

   void load(Serializer& serializer);

   // -------------------------------------------------------------------------
   // Entity implementation
   // -------------------------------------------------------------------------
protected:
   void registerProperties();
};

///////////////////////////////////////////////////////////////////////////////
