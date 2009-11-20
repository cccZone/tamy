#pragma once

/// @file   core-Scene\RendererView.h
/// @brief  view that renders model's contents on screen using Tamy renderer

#include "core-Scene\ModelView.h"
#include "core\GenericFactory.h"
#include "core\ResourceStorage.h"
#include "ext-RendererView\SceneManagers.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class GeometryRenderingMechanism;
class GraphicalEntitiesFactory;
class Renderer;
class Material;
class RenderingTarget;
class Camera;
class Texture;
class AbstractGraphicalEntity;

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
   ::Camera* m_camera;

   // resource storages
   ResourceStorage<Texture>* m_textures;
   ResourceStorage<::Material>* m_materials;
   ResourceStorage<AbstractGraphicalEntity>* m_dynamicEntities;

   // scene managers
   StaticSceneManager* m_staticGeometry;
   DynamicSceneManager* m_dynamicGeometry;
   LightsScene* m_lights;

   // rendering pipeline components
   GeometryRenderingMechanism* m_statSceneRenderer;
   GeometryRenderingMechanism* m_dynSceneRenderer;

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
    * The method sets a new static scene manager, replacing the old one.
    *
    * @param mgr  static scene manager instance
    */
   void setStaticSceneManager(StaticSceneManager* mgr);

   /**
    * This method gives access to static geometry management system used
    * by this view to aggregate the static geometry comprising the scene.
    *
    * @return  StaticSceneManager instance
    */
   StaticSceneManager& staticGeometry();

   /**
    * The method sets a new dynamic scene manager, replacing the old one.
    *
    * @param mgr  dynamic scene manager instance
    */
   void setDynamicSceneManager(DynamicSceneManager* mgr);

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
