#pragma once

#include "IWFEntityHandler.h"
#include <algorithm>
#include <string>
#include <vector>
#include "MeshDefinition.h"


///////////////////////////////////////////////////////////////////////////////

class IWFEntityHandlerMock : public IWFEntityHandler
{
private:
   std::string m_receivedLight;

   std::string m_receivedMesh;
   int m_subMeshesCount;

   std::vector<std::pair<std::string, std::string> > m_materials;
   std::set<std::string> m_materialNames;
   std::set<std::string> m_textureNames;
   std::set<std::string> m_skyboxTextureNames;

   std::string m_referenceName;

public:
   void addLight(const char* name, const MATRIX4& worldMtx, LIGHTENTITY& lightEntity)
   {
      m_receivedLight = name;
   }

   bool wasLoaded(const std::string& name) const
   {
      return m_receivedMesh == name;
   }

   void addMeshDefinition(const std::string& name, MeshDefinition& mesh)
   {
      m_receivedMesh = name;
      m_subMeshesCount = mesh.size();

      // deal with the materials
      for (MeshDefinition::const_iterator it = mesh.begin(); it != mesh.end(); ++it)
      {
         m_materialNames.insert(it->material.matName);
         m_textureNames.insert(it->material.texName);
      }
   }

   void addReference(const std::string& meshName,
                     const std::string& referenceName,
                     const D3DXMATRIX& worldMtx)
   {
   }

   void addSkybox(const std::string& frontTexName,
                  const std::string& backTexName,
                  const std::string& leftTexName,
                  const std::string& rightTexName,
                  const std::string& topTexName,
                  const std::string& bottomTexName)
   {
      m_skyboxTextureNames.insert(frontTexName);
      m_skyboxTextureNames.insert(backTexName);
      m_skyboxTextureNames.insert(leftTexName);
      m_skyboxTextureNames.insert(rightTexName);
      m_skyboxTextureNames.insert(topTexName);
      m_skyboxTextureNames.insert(bottomTexName);
   }

   void loadMesh(const std::string& meshName) {}

   int getReceivedMeshesCount() const {return m_subMeshesCount;}

   bool wasLightReceived(const std::string& name) {return m_receivedLight == name;}

   bool wasSkyboxTextureReceived(const std::string& name) const
   {
      std::set<std::string>::const_iterator it = m_skyboxTextureNames.find(name);
      return (it != m_skyboxTextureNames.end());
   }

   bool wasTextureNameReceived(const std::string& name) const
   {
      std::set<std::string>::const_iterator it = m_textureNames.find(name);
      return (it != m_textureNames.end());
   }

   bool wasMaterialNameReceived(const std::string& name) const
   {
      std::set<std::string>::const_iterator it = m_materialNames.find(name);
      return (it != m_materialNames.end());
   }
};

///////////////////////////////////////////////////////////////////////////////
