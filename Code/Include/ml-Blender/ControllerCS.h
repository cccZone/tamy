/// @file   ml-Blender/ControllerCS.h
/// @brief  a slice that represents an entity controller

#pragma once

#include "ml-Blender/IColladaSlice.h"
#include <vector>
#include "core-Renderer/LitVertex.h"
#include "core-Renderer/SkinnedGeometry.h"


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class TiXmlNode;
class Skeleton;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class ControllerCS : public IColladaSlice, public IEntitySlice
{
private:
   std::string                      m_skinSource;
   Skeleton*                        m_skeleton;

public:
   ControllerCS( TiXmlNode* geometryNode, ResourcesManager& rm );

   // -------------------------------------------------------------------------
   // IEntitySlice implementation
   // -------------------------------------------------------------------------
   Entity* instantiate( const BlenderScene& host ) const;
};

///////////////////////////////////////////////////////////////////////////////
