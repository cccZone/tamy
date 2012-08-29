/// @file   ml-Collada/IColladaSlice.h
/// @brief  a slice representing surface properties

#pragma once

#include "ml-Collada/IColladaSlice.h"
#include <string>


///////////////////////////////////////////////////////////////////////////////

class SurfaceProperties;
class Texture;
class Shader;
class ResourcesManager;
class TiXmlNode;
class FilePath;
class TiXmlElement;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class SurfacePropertiesCS : public IColladaSlice, public TResourceSlice< SurfaceProperties >
{
private:
   SurfaceProperties*      m_surfaceProperties;

public:
   SurfacePropertiesCS( TiXmlNode* effectNode, const FilePath& deploymentDir, ResourcesManager& rm );
   ~SurfacePropertiesCS();

protected:
   // -------------------------------------------------------------------------
   //  TResourceSlice< SurfaceProperties > implementation
   // -------------------------------------------------------------------------
   void* getResourcePtr() const { return m_surfaceProperties; }

private:
   // -------------------------------------------------------------------------
   //  Material shading techniques parsers
   // -------------------------------------------------------------------------
   void parseLambert( TiXmlElement* techniqueRoot );
   void parsePhong( TiXmlElement* techniqueRoot );
};

///////////////////////////////////////////////////////////////////////////////
