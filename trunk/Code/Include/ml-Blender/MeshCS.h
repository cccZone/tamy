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
class MeshCS : public IColladaSlice, public IEntitySlice
{
private:
   TriangleMesh*  m_mesh;

public:
   MeshCS( TiXmlNode* geometryNode, ResourcesManager& rm );

   // -------------------------------------------------------------------------
   // IEntitySlice implementation
   // -------------------------------------------------------------------------
   Entity* instantiate( const BlenderScene& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
