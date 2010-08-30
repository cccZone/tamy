/// @file   ml-Blender/IColladaSlice.h
/// @brief  a slice representing a visual effect

#pragma once

#include "ml-Blender/IColladaSlice.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Shader;
class ResourcesManager;
class TiXmlNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class EffectCS : public IColladaSlice, public IEntitySlice
{
private:
   Shader*        m_shader;
   std::string    m_materialId;

public:
   EffectCS( TiXmlNode* effectNode, ResourcesManager& rm );
   ~EffectCS();

   // -------------------------------------------------------------------------
   // IEntitySlice implementation
   // -------------------------------------------------------------------------
   Entity* instantiate( const BlenderScene& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
