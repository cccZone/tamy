#include "core-ResourceManagement\StaticGeometryLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "core-ResourceManagement\StaticGeometry.h"
#include "core-ResourceManagement\MeshHierarchyFlattener.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\Material.h"
#include <sstream>
#include <deque>


///////////////////////////////////////////////////////////////////////////////

StaticGeometryLoader::StaticGeometryLoader(GraphicalEntitiesFactory& factory)
: m_factory(factory)
{
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometryLoader::load(ResourceStorage<Material>& materialsStorage,
                                GraphicalDataSource& source,
                                StaticGeometry& geometryStorage)
{
   MeshDefinition rootMesh;
   AnimationDefinition animation;
   std::vector<MaterialDefinition> materials;

   source.parseMesh(rootMesh, animation, materials, "staticGeometry");

   MeshHierarchyFlattener meshExtractor;
   std::vector<FlatMesh*> meshes;
   meshExtractor(rootMesh, meshes);

   unsigned int materialsCount = materials.size();
   for (unsigned int matIdx = 0; matIdx < materialsCount; ++matIdx)
   {
      Material& material = createMaterial(materials[matIdx], 
                                          materialsStorage);

      for (std::vector<FlatMesh*>::iterator it = meshes.begin();
           it != meshes.end(); ++it)
      {
         if ((*it)->material == materials[matIdx].matName)
         {
            geometryStorage.addMesh((*it)->vertices, (*it)->faces, material);
            delete *it;
            meshes.erase(it);
            break;
         }
      }
   }

   // cleanup
   for (std::vector<FlatMesh*>::iterator it = meshes.begin();
        it != meshes.end(); ++it)
   {
      delete *it;
   }
}

///////////////////////////////////////////////////////////////////////////////

Material& StaticGeometryLoader::createMaterial(MaterialDefinition& material,
                                               ResourceStorage<Material>& materialsStorage)
{
   if (materialsStorage.is(material.matName) == false)
   {
      LightReflectingProperties* lrp  = m_factory.createLightReflectingProperties();
      lrp->setAmbientColor(material.ambient);
      lrp->setDiffuseColor(material.diffuse);
      lrp->setSpecularColor(material.specular);
      lrp->setEmissiveColor(material.emissive);
      lrp->setPower(material.power);

      MaterialStage* stage = m_factory.createMaterialStage(material.texName,
                                                           material.colorOp, 
                                                           material.colorArg1, 
                                                           material.colorArg2,
                                                           material.alphaOp, 
                                                           material.alphaArg1, 
                                                           material.alphaArg2,
                                                           material.coordsOp);
      Material* mat = m_factory.createMaterial(material.matName, lrp);
      mat->addStage(stage);
      materialsStorage.add(mat);

      return *mat;
   }
   else
   {
      return materialsStorage.get(material.matName);
   }
}

//////////////////////////////////////////////////////////////////////////////
