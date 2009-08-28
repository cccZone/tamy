#pragma once

/// @file   core-Renderer\RenderingPipelineBuilder.h
/// @brief  a language and framework for assembling rendering pipelines

#include "core\GraphBuilder.h"
#include "core\GraphBuilderFactory.h"
#include "core-Renderer\RenderingGraph.h"


///////////////////////////////////////////////////////////////////////////////

class RenderingPipelineDefinition;
class RenderingPipelineBuilder;
typedef GraphBuilder<RenderingNode, RenderingEdge> RenderingGraphBuilder;
class RenderingMechanismLeaf;
class SettableRenderingTargetsPolicy;
class RenderingTarget;

///////////////////////////////////////////////////////////////////////////////

/**
 * A helper class that controls the creation of a single rendering
 * mechanism.
 * It's a controller that forwards all commands back to RenderingPipelineBuilder.
 * I was introduced to enable the creation of a cool user interface that resembles
 * a dot-language.
 */
class RenderingPipelineTransform
{
private:
   RenderingPipelineBuilder& m_parent;
   std::string m_transformID;

   unsigned int m_inputsCount;
   unsigned int m_outputsCount;

public:
   /**
    * Constructor.
    *
    * @param parent        buidler we will execute the commands on
    * @param transformID   name of the rendering pipeline element
    */
   RenderingPipelineTransform(RenderingPipelineBuilder& parent,
                              const std::string& transformID);

   /**
    * The method defines a dependency on other element
    * of the pipeline. This is the way to instruct the builder
    * where to place this piece in the assembled pipeline
    * so that it's executed after the one it depends on.
    *
    * @param argID         id of an input to this element
    * @return              returns a reference to this instance, thus
    *                      allowing to chain other method calls
    */
   RenderingPipelineTransform& dep(const std::string& argID);

   /**
    * The method defines an input to this rendering element.
    *
    * Input to a rendering pipeline element is simply a storage
    * with renderables.
    *
    * RenderingPipelineDefinition associated with the builder 
    * should be able to find a match for this input, otherwise
    * the assembly process will fail.
    *
    * @param argID         id of an input to this element
    * @return              returns a reference to this instance, thus
    *                      allowing to chain other method calls
    */
   RenderingPipelineTransform& in(const std::string& argID);

   /**
    * The method defines an output from this rendering element.
    *
    * A rendering pipeline element outputis nothing more than 
    * a rendering target.
    *
    * RenderingPipelineDefinition associated with the builder 
    * should be able to find a match for this output, otherwise
    * the assembly process will fail.
    *
    * @param argID         id of an input to this element
    * @param passIdx       index of a rendering pass this target 
    *                      should be set for
    * @return              returns a reference to this instance, thus
    *                      allowing to chain other method calls
    */
   RenderingPipelineTransform& out(const std::string& argID, 
                                   unsigned int passIdx = 0);

   /** 
    * One needs to call this method when he finished defining the 
    * pipeline element. It will end the process and allow to define
    * other pipeline elements.
    *
    * @return              returns the control back to the main builder
    */
   RenderingPipelineBuilder& end();
};

///////////////////////////////////////////////////////////////////////////////

/** 
 * A builder that can assemble an entire rendering pipeline and
 * exposes a neat language-like interface that allows to configure
 * the entire pipeline that is to be created.
 */
class RenderingPipelineBuilder : public GraphBuilderFactory<RenderingNode, RenderingEdge>
{
   friend class RenderingPipelineTransform;

private:
   struct MechDef
   {
      PipelineChunk* chunk;
      RenderingMechanismLeaf* mechanism;
      SettableRenderingTargetsPolicy* policy;

      MechDef(PipelineChunk* _chunk,
         RenderingMechanismLeaf* _mechanism,
         SettableRenderingTargetsPolicy* _policy)
         : chunk(_chunk)
         , mechanism(_mechanism)
         , policy(_policy)
      {}
   };

private:
   RenderingPipelineDefinition& m_definition;

   std::vector<RenderingPipelineTransform*> m_transforms;

   typedef std::map<std::string, MechDef> MechsMap;
   MechsMap m_mechs;

   // this vector holds the dependencies in the form (mechanism, source)
   // mechanism for the source can be looked up using m_mechForSource map
   std::vector<std::pair<std::string, std::string> > m_dependencies; 

   // this set stores pointers to all the rendering targets used
   // by a particular pipeline. The set is populated during pipeline
   // assembly process.
   typedef std::set<RenderingTarget*> AllTargetsSet;
   AllTargetsSet m_allTargets;
   SettableRenderingTargetsPolicy* m_cleanerPolicy;

   RenderingGraph m_renderGraph;
   RenderingGraphBuilder* m_graphBuilder;

public:
   //--------------------------------------------------------------------------
   // public interface
   //--------------------------------------------------------------------------

   /** 
    * Constructor.
    *
    * @param definition    a supplier that will feed this class with
    *                      elements required during the pipeline assembly process
    */
   RenderingPipelineBuilder(RenderingPipelineDefinition& definition);

   ~RenderingPipelineBuilder();

   /**
    * Opens a block of code that allows to define a new pipeline 
    * rendering element.
    *
    * @param transformID      an id of the new element
    */
   RenderingPipelineTransform& defineTransform(const std::string& transformID);

   /**
    * Ends the pipeline building process, returning a mechanism that 
    * contains the entire pipeline.
    */
   RenderingMechanism* end();

   //--------------------------------------------------------------------------
   // GraphBuilderFactory implementation
   //--------------------------------------------------------------------------

   RenderingNode createStartNode();

   RenderingNode createNode(const std::string& name);

   RenderingEdge createEdge(RenderingGraph::Index endNodeIdx);

private:
   //--------------------------------------------------------------------------
   // interface for RenderingPipelineTransform
   //--------------------------------------------------------------------------
   void createMechanism(const std::string& mechName);

   void defineDependency(const std::string& mechanismName, 
                         const std::string& depMechanismName);

   void assignSource(MechDef& mech, const std::string& sourceName);

   void addRenderTarget(SettableRenderingTargetsPolicy& policy, 
                        unsigned int passIdx,
                        const std::string& targetName);

   MechDef& getMechDef(const std::string& mechName);
};

///////////////////////////////////////////////////////////////////////////////
