/// @file   ml-Collada/SceneCS.h
/// @brief  a slice that represents a scene object

#pragma once

#include "ml-Collada/IColladaSlice.h"
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ResourcesManager;
class TiXmlNode;
class TiXmlElement;
class SpatialEntity;
class FilePath;

///////////////////////////////////////////////////////////////////////////////

/**
* An instantiator of triangle mesh resource.
    */
 class SceneCS : public IColladaSlice, public ISceneSlice
{
private:
   struct NodeDef
   {
      TiXmlElement*              nodeElem;
      std::vector< NodeDef* >    children;

      NodeDef( TiXmlElement* elem );
      ~NodeDef();

      SpatialEntity* instantiate( const ColladaScene& scene, const SceneCS& hostSlice ) const;

   private:
      void parseMaterial( TiXmlElement& elem, const ColladaScene& scene, Entity* entity ) const;
      void parseSkeletons( TiXmlElement& elem, const SceneCS& hostSlice, const ColladaScene& scene, Entity* entity ) const;
   };
   friend struct NodeDef;

private:
   ResourcesManager&                m_rm;
   std::vector< NodeDef* >          m_nodes;

public:
   SceneCS( TiXmlNode* meshNode, const FilePath& deploymentDir, ResourcesManager& rm );
   ~SceneCS();

   // -------------------------------------------------------------------------
   // IEntitySlice implementation
   // -------------------------------------------------------------------------
   void instantiate( const ColladaScene& host, Model& scene ) const;

private:
   NodeDef* createNode( TiXmlElement* nodeElem );
   NodeDef* findNode( const std::string& id ) const;
};

///////////////////////////////////////////////////////////////////////////////
