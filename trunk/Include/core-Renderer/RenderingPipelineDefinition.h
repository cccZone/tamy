#pragma once

/// @file   core-Renderer\RenderingPipelineDefinition.h
/// @brief  supplier of data to the RenderingPipelineBuilder

#include <string>


///////////////////////////////////////////////////////////////////////////////

class SettableRenderingTargetsPolicy;
class RenderingMechanismLeaf;
class RenderingMechanism;
class RenderablesStorage;
class RenderingTarget;
class RenderingTargetsPolicy;

///////////////////////////////////////////////////////////////////////////////

/**
 * Defines the building blocks the rendering pipeline can be composed of.
 * It can be thought of as a database and a factory of such building blocks.
 *
 * The recipe for the pipeline is defined elsewhere - this is just a supplier.
 */
class RenderingPipelineDefinition
{
public:
   virtual ~RenderingPipelineDefinition() {}

   /**
    * Creates a rendering mechanism corresponding to the specified id.
    *
    * @param mechName      id of the mechanism we want to create
    * @param policy        an instance of the rendering targets policy the mechanism
    *                      should employ
    * @param outMechanism  this is an output argument. Upon method return it should
    *                      contain a pointer to the newly created rendering mechanism.
    */
   virtual void createMechanism(const std::string& mechName, 
                                SettableRenderingTargetsPolicy* policy,
                                RenderingMechanismLeaf** outMechanism) = 0;

   /**
    * This method is responsible for creating a rendering mechanism
    * that will take care of cleaning the rendering targets the policy
    * it's injected with has.
    *
    * @param policy        an instance of the rendering targets policy the mechanism
    *                      should employ
    */
   virtual RenderingMechanism* createTargetsCleaner(RenderingTargetsPolicy* policy) = 0;

   /**
    * Creates a storage that corresponds to the specified id.
    * The storage should already be set up with all the data it needs to return.
    *
    * @param sourceName    id of the storage we want created
    * @return              a new instance of the storage.
    */
   virtual RenderablesStorage* createStorage(const std::string& sourceName) = 0;

   /**
    * Returns a reference to a rendering target that coresponds 
    * to the specified id.
    *
    * @param targetName    id of the target we want
    * @return              a reference to the requested target
    */
   virtual RenderingTarget& getRenderTarget(const std::string& targetName) = 0;
};

///////////////////////////////////////////////////////////////////////////////
