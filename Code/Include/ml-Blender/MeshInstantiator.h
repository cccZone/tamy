/// @file   ml-Blender/MeshInstantiator.h
/// @brief  an instantiator of triangle mesh resource

#pragma once

#include "ml-Blender/IResourceInstantiator.h"


///////////////////////////////////////////////////////////////////////////////

class TriangleMesh;
class ResourcesManager;
class TiXmlNode;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class MeshInstantiator : public IResourceInstantiator
{
private:
   TriangleMesh*  m_mesh;

public:
   MeshInstantiator( TiXmlNode* meshNode, ResourcesManager& rm );

   // -------------------------------------------------------------------------
   // IResourceInstantiator implementation
   // -------------------------------------------------------------------------
   Entity* instantiate() const;
};

///////////////////////////////////////////////////////////////////////////////
