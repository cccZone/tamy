/// @file   ml-Collada/MeshCS.h
/// @brief  a slice that represents a triangle mesh resource

#pragma once

#include "ml-Collada/IColladaSlice.h"


///////////////////////////////////////////////////////////////////////////////

class TriangleMesh;
class ResourcesManager;
class TiXmlNode;
class FilePath;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class MeshCS : public IColladaSlice, public TResourceSlice< TriangleMesh >
{
private:
   TriangleMesh*  m_mesh;

public:
   MeshCS( TiXmlNode* geometryNode, const FilePath& deploymentDir, ResourcesManager& rm );

protected:
   // -------------------------------------------------------------------------
   //  TResourceSlice< TriangleMesh > implementation
   // -------------------------------------------------------------------------
   void* getResourcePtr() const { return m_mesh; }

};

///////////////////////////////////////////////////////////////////////////////
