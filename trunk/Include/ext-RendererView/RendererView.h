#pragma once

/// @file   core-Scene\RendererView.h
/// @brief  view that renders model's contents on screen using Tamy renderer

#include "core-Scene\ModelView.h"
#include "core\GenericFactory.h"
#include "core\ResourceStorage.h"
#include "core\LinearStorage.h"
#include "core\TNodesSpatialStorage.h"
#include "core\StaticGeometryOctree.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class Renderer;
class Material;
class RenderingTarget;
class Camera;
class Texture;
class AbstractGraphicalEntity;
class RenderableNode;
typedef TNodesSpatialStorage<RenderableNode, LinearStorage> DynamicSceneManager;
class StaticGeometryRenderable;
typedef StaticGeometryOctree<StaticGeometryRenderable> StaticSceneManager;

///////////////////////////////////////////////////////////////////////////////

namespace RendererView
{

///////////////////////////////////////////////////////////////////////////////

class GraphicalRepresentation;
class LightsScene;

///////////////////////////////////////////////////////////////////////////////

class RendererView : public ModelView, 
                     public GenericFactory<Entity, GraphicalRepresentation>
{
private:
   GraphicalEntitiesFactory& m_factory;
   Renderer& m_renderer;

   typedef std::map<Entity*, GraphicalRepresentation*> Representations;
   Representations m_representations;

   RenderingTarget* m_screenTarget;
   LightsScene* m_lights;
   ResourceStorage<Texture>* m_textures;
   ResourceStorage<::Material>* m_materials;
   ResourceStorage<AbstractGraphicalEntity>* m_dynamicEntities;
   StaticSceneManager* m_staticGeometry;
   DynamicSceneManager* m_dynamicGeometry;
   ::Camera* m_camera;

public:
   /**
    * Constructor.
    *
    * @param factory    factory capable of creating basic rendering entities
    * @param renderer   renderer instance
    */
   RendererView(GraphicalEntitiesFactory& factory,
                Renderer& renderer);
   ~RendererView();

   /**
    * This method updates the state of representations
    * created in this view.
    *
    * @param timeElapsed   amount of time that has elapsed since the last time
    *                      this method was called
    */
   void update(float timeElapsed);

   // -------------------------------------------------------------------------
   // Resources
   // -------------------------------------------------------------------------
   /**
    * The method returns a factory that can be used to create various
    * rendering tools.
    *
    * @return  GraphicalEntitiesFactory instance
    */
   GraphicalEntitiesFactory& factory();

   /**
    * The method returns a light scene used by the renderer.
    *
    * @return  LightsScene instance
    */
   LightsScene& lights();

   /**
    * The method returns a common storage of materials used by renderer.
    *
    * @return  ResourceStorage<Material> instance
    */
   ResourceStorage<::Material>& materials();

   /**
    * The method returns a common storage of textures used by renderer.
    *
    * @return  ResourceStorage<Texture> instance
    */
   ResourceStorage<Texture>& textures();

   /**
    * The method returns a common storage of dynamic entities used by renderer.
    *
    * @return  ResourceStorage<AbstractGraphicalEntity> instance
    */
   ResourceStorage<AbstractGraphicalEntity>& dynamicEntities();

   /**
    * This method gives access to static geometry management system used
    * by this view to aggregate the static geometry comprising the scene.
    *
    * @return  StaticSceneManager instance
    */
   StaticSceneManager& staticGeometry();

   /**
    * This method gives access to dynamic geometry management system used
    * by this view to aggregate the dynamic objects comprising the scene.
    *
    * @return  DynamicSceneManager instance
    */
   DynamicSceneManager& dynamicGeometry();

   /**
    * Returns an instance of Camera used by the renderer.
    *
    * @return ::Camera instance
    */
   ::Camera& camera();

   // -------------------------------------------------------------------------
   // ModelView implementation
   // -------------------------------------------------------------------------
   void onEntityAdded(Entity& entity);

   void onEntityRemoved(Entity& entity);
   
protected:
   void resetContents();
};

///////////////////////////////////////////////////////////////////////////////

} // ScenePlayer
