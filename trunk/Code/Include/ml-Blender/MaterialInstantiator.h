/// @file   ml-Blender/MaterialInstantiator.h
/// @brief  an instantiator of material resource

#pragma once

#include "ml-Blender/IResourceInstantiator.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class Material;
class Texture;
class Shader;
class ResourcesManager;
class TiXmlNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class MaterialInstantiator : public IResourceInstantiator
{
private:
   std::string    m_name;
   Material*      m_material;
   Texture*       m_texture;
   Shader*        m_shader;

public:
   MaterialInstantiator( TiXmlNode* materialNode, ResourcesManager& rm );
   ~MaterialInstantiator();

   // -------------------------------------------------------------------------
   // IResourceInstantiator implementation
   // -------------------------------------------------------------------------
   Entity* instantiate() const;
};

///////////////////////////////////////////////////////////////////////////////
