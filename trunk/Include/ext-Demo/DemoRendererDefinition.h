#pragma once

/// @file   ext-Demo\DemoRendererDefinition.h
/// @brief  renderer structure definitions provider for demos

#include "core-Renderer\RenderingPipelineDefinition.h"
#include <map>
#include <set>


///////////////////////////////////////////////////////////////////////////////

class GraphicalEntitiesFactory;
class TextureRenderingTarget;
class RenderingTarget;
class Camera;
class LightsStorage;
class RenderingAction;
class SkyBoxStorage;
class Texture;

///////////////////////////////////////////////////////////////////////////////

namespace demo
{

///////////////////////////////////////////////////////////////////////////////

class RERCreator;

///////////////////////////////////////////////////////////////////////////////

enum TargetType
{
   RT_COLOR,
   RT_NORMAL,
   RT_DEPTH
};

///////////////////////////////////////////////////////////////////////////////

/**
 * This class will provide the definitions requred be the rendering
 * pipeline builder when it's used in demos.
 */
class DemoRendererDefinition : public RenderingPipelineDefinition
{
private:
   GraphicalEntitiesFactory& m_factory;
   RenderingTarget* m_backBuffer;

   typedef std::map<std::string, std::string> MechanismAliasesMap;
   MechanismAliasesMap m_mechAliases;

   typedef std::map<std::string, RERCreator*> RERCreatorsMap;
   RERCreatorsMap m_rerCreators;

   typedef std::map<std::string, TextureRenderingTarget*> RenderingTargetsMap;
   RenderingTargetsMap m_targets;

   typedef std::map<std::string, RenderablesStorage*> SharedStoragesMap;
   SharedStoragesMap m_storages;

   typedef std::map<std::string, LightsStorage*> LightStoragesMap;
   LightStoragesMap m_lights;
   std::set<LightsStorage*> m_lightStoragesMgmt;

   typedef std::map<std::string, Camera*> CamerasMap;
   CamerasMap m_cameras;

public:
   DemoRendererDefinition(GraphicalEntitiesFactory& factory);
   ~DemoRendererDefinition();

   /**
    * Assigns an alias to the mechanism. An alias allows to reference
    * the same mechanism using different names - thus allowing to
    * assign the same mechanism use different light storages
    * for example.
    */
   void aliasMechanism(const std::string& mechName, const std::string& alias);

   /**
    * Sets a new background for rendering.
    * Background is a data source and can be accessed using the tag
    * <<background>>
    */
   void setBackground(SkyBoxStorage* background);

   /** 
    * Registers a new renderables source definition.
    * 
    * @param sourceID   id under which the builder can access this source
    * @param source     the source itself
    */
   void addSource(const std::string& sourceID, RenderablesStorage* source);

   /** 
    * Registers a new storage of lights that should be used
    * to light the scene composed of the renderables from the specified
    * source.
    * 
    * @param mechName   id of the mechanism we want to assign a light
    *                   storage to (can be an alias)
    * @param lights     storage with lights for this mechanism
    */
   void addLightsForMechanism(const std::string& mechName, 
                              LightsStorage* lights);

   /**
    * You need to call this method if you wish to use rendering mechanism other
    * than <<fixedPipeline>> - namely if you wish to load shaders :)
    *
    * Pass the name of the shader here, along with a creator that creates
    * a RenderingEffectRenderer instance that operates on that shader here,
    * and the builder will take care of the rest.
    *
    * @param mechName      name of a file that contains the shaders we want
    *                      to use as a rendering mechanism
    * @param creator       a creator that will create a RenderingEffectRenderer
    *                      instance applicable to the created effect
    */
   void setProgrammableMechanismLoader(const std::string& mechName, 
                                       RERCreator* creator);

   /** 
    * Creates a new rendering target.
    * 
    * @param targetID   id under which the builder can access this target
    * @param type       type of the target (screen buffer, stencil texture,
    *                   color texture etc.)
    */
   void createTarget(const std::string& targetID, TargetType type);

   /**
    * The method allows to retrieve an internally rendering target
    * as a texture, which can be used as a source to create env effects
    * for instance, or for post process effects...
    *
    * @param targetID   id of a rendering target contents of which
    *                   we want to use as a texture. Specifying "<<screen>>"
    *                   here will raise an exception.
    * @return           texture with the rendering target contents
    */
   Texture& getTargetAsTexture(const std::string& targetID);

   /** 
    * Registers a camera that should be used to render the contents
    * of the specified mechanism.
    * 
    * @param mechName   id of the mechanism we want to assign the camera to
    *                   (can be an alias)
    * @param camera     the camera itself
    */
   void defineCamera(const std::string& mechName, Camera& camera);

   // -------------------------------------------------------------------------
   // RenderingPipelineDefinition implementation
   // -------------------------------------------------------------------------

   void createMechanism(const std::string& mechName, 
                        SettableRenderingTargetsPolicy* policy,
                        RenderingMechanismLeaf** outMechanism);

   RenderingMechanism* createTargetsCleaner(RenderingTargetsPolicy* policy);

   RenderablesStorage* createStorage(const std::string& sourceName);

   RenderingTarget& getRenderTarget(const std::string& targetName);

private:
   std::string unaliasMechName(const std::string& alias) const;
};

///////////////////////////////////////////////////////////////////////////////

} // namespace demo
