#include "ext-Demo\DemoRendererDefinition.h"
#include "core-Renderer\RenderingMechanismLeaf.h"
#include "core-Renderer\GraphicalEntitiesFactory.h"
#include "core-Renderer\RenderablesStorage.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\RenderingActionsComposite.h"
#include "core-Renderer\RenderMaterial.h"
#include "core-Renderer\RenderLights.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\RenderingEffectRenderer.h"
#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core-Renderer\TextureRenderingTarget.h"
#include "ext-Demo\RERCreator.h"


using namespace demo;

///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class SharedRenderablesStorage : public RenderablesStorage
   {
   private:
      RenderablesStorage& m_storage;

   public:
      SharedRenderablesStorage(RenderablesStorage& storage)
         : m_storage(storage)
      {}

      void query(Camera& camera, Array<Renderable*>& outNodes) 
      {
         m_storage.query(camera, outNodes);
      }
   };

   // -------------------------------------------------------------------------

   class NullRenderablesStorage : public RenderablesStorage
   {
   public:
      void query(Camera& camera, Array<Renderable*>& outNodes) {}
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

DemoRendererDefinition::DemoRendererDefinition(GraphicalEntitiesFactory& factory)
: m_factory(factory)
{
   m_backBuffer = m_factory.createDefaultRenderingTarget();

   m_storages.insert(std::make_pair("<<background>>", new NullRenderablesStorage()));
}

///////////////////////////////////////////////////////////////////////////////

DemoRendererDefinition::~DemoRendererDefinition()
{
   for (SharedStoragesMap::iterator it = m_storages.begin();
        it != m_storages.end(); ++it)
   {
      delete it->second;
   }
   m_storages.clear();

   for (std::set<LightsStorage*>::iterator it = m_lightStoragesMgmt.begin();
        it != m_lightStoragesMgmt.end(); ++it)
   {
      delete *it;
   }
   m_lights.clear();
   m_lightStoragesMgmt.clear();

   for (RERCreatorsMap::iterator it = m_rerCreators.begin();
        it != m_rerCreators.end(); ++it)
   {
      delete it->second;
   }
   m_rerCreators.clear();
   m_targets.clear();
   m_cameras.clear();
   m_mechAliases.clear();

   delete m_backBuffer;
   m_backBuffer = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::aliasMechanism(const std::string& mechName, 
                                            const std::string& alias)
{
   MechanismAliasesMap::iterator it = m_mechAliases.find(alias);
   if (it != m_mechAliases.end())
   {
      throw std::invalid_argument(std::string("Alias ") + alias + " already defined");
   }

   m_mechAliases.insert(std::make_pair(alias, mechName));
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::setBackground(SkyBoxStorage* background)
{
   if (background == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a SkyBoxStorage instance");
   }

   delete m_storages["<<background>>"];
   m_storages["<<background>>"] = background;
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::addSource(const std::string& sourceID, 
                                       RenderablesStorage* source)
{
   if (source == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RenderablesStorage instance");
   }

   SharedStoragesMap::iterator it = m_storages.find(sourceID);
   if (it != m_storages.end())
   {
      throw std::invalid_argument(std::string("Source ") + sourceID + " definition already exists");
   }

   m_storages.insert(std::make_pair(sourceID, source));
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::addLightsForMechanism(const std::string& mechName, 
                                                   LightsStorage* lights)
{
   if (lights == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a LightsStorage instance");
   }

   LightStoragesMap::iterator it = m_lights.find(mechName);
   if (it != m_lights.end())
   {
      throw std::invalid_argument(std::string("Mechanism ") + mechName + " already has light storage assigned");
   }

   m_lights.insert(std::make_pair(mechName, lights));
   m_lightStoragesMgmt.insert(lights);
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::setProgrammableMechanismLoader(const std::string& mechName, 
                                                            RERCreator* creator)
{
   if (creator == NULL)
   {
      throw std::invalid_argument("NULL pointer instead a RERCreator instance");
   }

   if (mechName == "<<fixedPipeline>>")
   {
      throw std::invalid_argument("Can't define a creator for a predefined mechanism <<fixedPipeline>>");
   }

   RERCreatorsMap::iterator it = m_rerCreators.find(mechName);
   if (it != m_rerCreators.end())
   {
      throw std::invalid_argument(std::string("Creator for mechanism") + mechName + " already defined");
   }

   m_rerCreators.insert(std::make_pair(mechName, creator));
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::createTarget(const std::string& targetID, 
                                          TargetType type)
{
   TextureRenderingTarget* target = m_factory.createTextureRenderingTarget(targetID);
   m_factory.getTexturesStorage().add(target);

   RenderingTargetsMap::iterator it = m_targets.find(targetID);
   if (it != m_targets.end())
   {
      throw std::invalid_argument(std::string("Target ") + targetID + " definition already exists");
   }

   m_targets.insert(std::make_pair(targetID, target));
}

///////////////////////////////////////////////////////////////////////////////

Texture& DemoRendererDefinition::getTargetAsTexture(const std::string& targetID)
{
   RenderingTargetsMap::iterator it = m_targets.find(targetID);
   if (it == m_targets.end())
   {
      throw std::invalid_argument(std::string("Target ") + targetID + " is not defined");
   }

   return *(it->second);
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::defineCamera(const std::string& mechName, 
                                          Camera& camera)
{
   CamerasMap::iterator it = m_cameras.find(mechName);
   if (it != m_cameras.end())
   {
      throw std::invalid_argument(std::string("Camera already assigned to ") + mechName + " mechanism");
   }

   m_cameras.insert(std::make_pair(mechName, &camera));
}

///////////////////////////////////////////////////////////////////////////////

void DemoRendererDefinition::createMechanism(const std::string& mechName, 
                                             SettableRenderingTargetsPolicy* policy,
                                             RenderingMechanismLeaf** outMechanism)
{
   RendererImpl* impl = NULL;

   // define rendering actions
   RenderingActionsComposite* renderingAction = new RenderingActionsComposite();
   renderingAction->addAction(new RenderMaterial());

   LightStoragesMap::iterator lightsIt = m_lights.find(mechName);
   if (lightsIt != m_lights.end())
   {
      RenderLights* lightsAction = new RenderLights(*(lightsIt->second));
      renderingAction->addAction(lightsAction);
   }

   // create the mechanism
   std::string realMechName = unaliasMechName(mechName);
   if (realMechName == "<<fixedPipeline>>")
   {
      impl = m_factory.createFixedRendererImpl();
   }
   else
   {

      RERCreatorsMap::iterator rerCreatorIt = m_rerCreators.find(realMechName);
      if (rerCreatorIt == m_rerCreators.end())
      {
         throw std::runtime_error(std::string("Creator for mechanism ") + realMechName + " not found");
      }

      GraphicalEffect* effect = m_factory.createEffect(realMechName);
      impl = rerCreatorIt->second->create(effect);
   }

   *outMechanism = new RenderingMechanismLeaf(policy, impl);
   (*outMechanism)->setRenderingAction(renderingAction);

   // find the camera for the mechanism
   CamerasMap::iterator cameraIt = m_cameras.find(mechName);
   if (cameraIt == m_cameras.end())
   {
      throw std::invalid_argument(std::string("Camera isn't defined for ") + mechName + " mechanism");
   }
   (*outMechanism)->setActiveCamera(*(cameraIt->second));
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* 
DemoRendererDefinition::createTargetsCleaner(RenderingTargetsPolicy* policy)
{
   RenderingTargetsCleaner* cleaner = m_factory.createTargetsCleaner();
   return new TargetsCleaningMechanism(policy, cleaner); 
}

///////////////////////////////////////////////////////////////////////////////

RenderablesStorage* 
DemoRendererDefinition::createStorage(const std::string& sourceName)
{
   SharedStoragesMap::iterator storageIt = m_storages.find(sourceName);
   if (storageIt == m_storages.end())
   {
      throw std::invalid_argument(std::string("Source ") + sourceName + " is not defined");
   }
   RenderablesStorage* storage = storageIt->second;
   if (storage == NULL)
   {
      throw std::runtime_error("NULL pointer instead a RenderablesStorage instance"); 
   }

   return new SharedRenderablesStorage(*storage);
}

///////////////////////////////////////////////////////////////////////////////

RenderingTarget&
DemoRendererDefinition::getRenderTarget(const std::string& targetName)
{
   if (targetName == "<<screen>>")
   {
      return *m_backBuffer;
   }
   else
   {
      RenderingTargetsMap::iterator it = m_targets.find(targetName);
      if (it == m_targets.end())
      {
         throw std::invalid_argument(std::string("Target ") + targetName + " is not defined");
      }
      
      return *(it->second);
   }
}

///////////////////////////////////////////////////////////////////////////////

std::string 
DemoRendererDefinition::unaliasMechName(const std::string& alias) const
{
   std::string realMechName = alias;
   MechanismAliasesMap::const_iterator mechAliasIt = m_mechAliases.find(alias);
   if (mechAliasIt != m_mechAliases.end())
   {
      realMechName = mechAliasIt->second;
   }

   return realMechName;
}

///////////////////////////////////////////////////////////////////////////////
