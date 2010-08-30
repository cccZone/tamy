/// @file   ml-Blender/SceneCS.h
/// @brief  a slice that represents a scene object

#pragma once

#include "ml-Blender/IColladaSlice.h"
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class TiXmlNode;
class TiXmlElement;
class SpatialEntity;

///////////////////////////////////////////////////////////////////////////////

/**
 * An instantiator of triangle mesh resource.
 */
class SceneCS : public IColladaSlice, public ISceneSlice
{
private:
   struct NodeDef
   {
      std::string                id;
      D3DXMATRIX                 localMtx;
      std::vector< std::string > geometryURI;
      std::vector< std::string > materialURI;

      std::vector< NodeDef* >    children;

      NodeDef( const std::string& _id );
      ~NodeDef();
      SpatialEntity* instantiate( const BlenderScene& scene ) const;
   };

private:
   ResourcesManager&                m_rm;
   std::vector< NodeDef* >          m_nodes;

public:
   SceneCS( TiXmlNode* meshNode, ResourcesManager& rm );
   ~SceneCS();

   // -------------------------------------------------------------------------
   // IEntitySlice implementation
   // -------------------------------------------------------------------------
   void instantiate( const BlenderScene& host, Model& scene ) const;

private:
   NodeDef* createNode( TiXmlElement* nodeElem );
   NodeDef* parseEntityNode( TiXmlElement* nodeElem );
};

///////////////////////////////////////////////////////////////////////////////

