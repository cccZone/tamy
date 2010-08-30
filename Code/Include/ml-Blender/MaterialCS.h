/// @file   ml-Blender/IColladaSlice.h
/// @brief  a slice representing a material

#pragma once

#include "ml-Blender/IColladaSlice.h"
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
class MaterialCS : public IColladaSlice, public TResourceSlice< Material >
{
private:
   Material*      m_material;

public:
   MaterialCS( TiXmlNode* effectNode, ResourcesManager& rm );
   ~MaterialCS();

protected:
   // -------------------------------------------------------------------------
   //  TResourceSlice< Material > implementation
   // -------------------------------------------------------------------------
   void* getResourcePtr() const { return m_material; }
};

///////////////////////////////////////////////////////////////////////////////
