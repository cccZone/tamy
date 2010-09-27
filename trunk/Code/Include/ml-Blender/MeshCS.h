/// @file   ml-Blender/MeshCS.h
/// @brief  a slice that represents a triangle mesh resource

#pragma once

#include "ml-Blender/IColladaSlice.h"


///////////////////////////////////////////////////////////////////////////////

class TriangleMesh;
class ResourcesManager;
class TiXmlNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class MeshCS : public IColladaSlice, public TResourceSlice< TriangleMesh >
{
private:
   TriangleMesh*  m_mesh;

public:
   MeshCS( TiXmlNode* geometryNode, ResourcesManager& rm );

protected:
   // -------------------------------------------------------------------------
   //  TResourceSlice< TriangleMesh > implementation
   // -------------------------------------------------------------------------
   void* getResourcePtr() const { return m_mesh; }

};

///////////////////////////////////////////////////////////////////////////////
