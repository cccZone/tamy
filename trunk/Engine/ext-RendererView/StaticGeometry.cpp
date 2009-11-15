#include "ext-RendererView\StaticGeometry.h"
#include "ext-RendererView\RendererView.h"
#include "core-Scene\StaticGeometryEntity.h"
#include "core-ResourceManagement\StaticGeometryLoader.h"
#include "core-ResourceManagement\GraphicalDataSource.h"
#include "ext-RendererView\LightsScene.h"
#include "core-Renderer\StaticGeometryRenderable.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\Light.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::StaticGeometry(StaticGeometryEntity& entity)
: m_entity(entity)
{
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometry::~StaticGeometry()
{
   m_geometry.clear();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::initialize(RendererView& rendererView)
{
   // ISSUE: nigdzie nie uzywamy macierzy z entity - jak bedziemy pozycjonowac statyczna geometrie?
   unsigned int count = m_entity.m_meshes.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      if (m_entity.m_meshes[i]->m_materials.size() != 1)
      {
         throw std::runtime_error("Static must have one material assigned in order to be rendered with this representation");
      }
      const std::string& materialName = m_entity.m_meshes[i]->m_materials[0];

      if (rendererView.materials().is(materialName) == true)
      {
         Material& material = rendererView.materials().get(materialName);

         StaticGeometryRenderable* geometry = rendererView.factory().createStaticGeometry(material, 
            m_entity.m_meshes[i]->m_vertices, 
            m_entity.m_meshes[i]->m_faces);
         m_geometry.push_back(rendererView.staticGeometry().insert(geometry));
      }
      else
      {
         throw std::runtime_error(std::string("Material ") + materialName + " is not defined");
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::deinitialize(RendererView& rendererView)
{
   unsigned int count = m_geometry.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      rendererView.staticGeometry().remove(m_geometry[i]);
   }
   m_geometry.clear();
}

///////////////////////////////////////////////////////////////////////////////

void StaticGeometry::update(float timeElapsed)
{
   // TODO
}

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
