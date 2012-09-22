/// @file   ml-Collada/IColladaSlice.h
/// @brief  a slice representing a visual effect

#pragma once

#include "ml-Collada/IColladaSlice.h"
#include <string>
#include "core/FilePath.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class TiXmlNode;
class FilePath;
class MaterialInstance;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class EffectCS : public IColladaSlice, public IEntitySlice
{
private:
   std::string             m_materialName;
   std::string             m_materialId;
   MaterialInstance*       m_materialInstance;

public:
   EffectCS( TiXmlNode* effectNode, const FilePath& deploymentDir, ResourcesManager& rm );
   ~EffectCS();

   // -------------------------------------------------------------------------
   // IEntitySlice implementation
   // -------------------------------------------------------------------------
   Entity* instantiate( const ColladaScene& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
