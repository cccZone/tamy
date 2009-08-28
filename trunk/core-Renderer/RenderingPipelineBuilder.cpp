#include "core-Renderer\RenderingPipelineBuilder.h"
#include "core-Renderer\RenderingPipelineDefinition.h"
#include "core-Renderer\RenderingMechanismBuilder.h"
#include "core-Renderer\RenderingMechanismLeaf.h"
#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core\GraphBFS.h"


///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBuilder::RenderingPipelineBuilder(RenderingPipelineDefinition& definition)
: m_definition(definition)
, m_cleanerPolicy(NULL)
{
   m_graphBuilder = new RenderingGraphBuilder(*this, m_renderGraph);
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBuilder::~RenderingPipelineBuilder()
{
   unsigned int count = m_transforms.size();
   for (unsigned int i = 0; i < count; ++i)
   {
      delete m_transforms[i];
   }
   m_transforms.clear();

   count = m_mechs.size();
   for (MechsMap::iterator it = m_mechs.begin(); it != m_mechs.end(); ++it)
   {
      delete it->second.chunk;
   }
   m_mechs.clear();

   delete m_graphBuilder;
   m_graphBuilder = NULL;

   m_cleanerPolicy = NULL;
}

///////////////////////////////////////////////////////////////////////////////

RenderingNode RenderingPipelineBuilder::createStartNode() 
{
   if (m_cleanerPolicy != NULL)
   {
      throw std::runtime_error("Build procedure is corrupt");
   }
   m_cleanerPolicy = new SettableRenderingTargetsPolicy();
   RenderingMechanism* mech = m_definition.createTargetsCleaner(m_cleanerPolicy);

   return RenderingNode(mech);
}

///////////////////////////////////////////////////////////////////////////////

RenderingNode RenderingPipelineBuilder::createNode(const std::string& name) 
{
   MechsMap::iterator it = m_mechs.find(name);
   if (it != m_mechs.end())
   {
      return RenderingNode(it->second.mechanism);
   }
   else
   {
      return RenderingNode();
   }
}

///////////////////////////////////////////////////////////////////////////////

RenderingEdge RenderingPipelineBuilder::createEdge(RenderingGraph::Index endNodeIdx) 
{
   return RenderingEdge(endNodeIdx);
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineTransform& 
RenderingPipelineBuilder::defineTransform(const std::string& transformID)
{
   RenderingPipelineTransform* newTransform = new RenderingPipelineTransform(*this, transformID);
   m_transforms.push_back(newTransform);

   return *newTransform;
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBuilder::createMechanism(const std::string& mechName)
{
   MechsMap::iterator depMechIt = m_mechs.find(mechName);
   if (depMechIt != m_mechs.end())
   {
      throw std::runtime_error(std::string("Mechanism") + mechName + " is already defined");
   }

   RenderingMechanismLeaf* mechanism = NULL;
   SettableRenderingTargetsPolicy* policy = new SettableRenderingTargetsPolicy();
   m_definition.createMechanism(mechName, policy, &mechanism);

   PipelineChunk* chunk = new PipelineChunk(mechName);
   m_mechs.insert(std::make_pair(mechName, MechDef(chunk, mechanism, policy)));
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBuilder::defineDependency(const std::string& mechanismName, 
                                                const std::string& depMechanismName)
{
   m_dependencies.push_back(std::make_pair(mechanismName, depMechanismName));
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBuilder::assignSource(MechDef& mech, 
                                            const std::string& sourceName)
{
   RenderablesStorage* storage = m_definition.createStorage(sourceName);
   mech.mechanism->setStorage(storage);
}

///////////////////////////////////////////////////////////////////////////////

void RenderingPipelineBuilder::addRenderTarget(SettableRenderingTargetsPolicy& policy, 
                                               unsigned int passIdx,
                                               const std::string& targetName)
{
   RenderingTarget& target = m_definition.getRenderTarget(targetName);
   policy.addTarget(passIdx, target);
   m_allTargets.insert(&target);
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBuilder::MechDef& 
RenderingPipelineBuilder::getMechDef(const std::string& mechName)
{
   MechsMap::iterator it = m_mechs.find(mechName);
   if (it == m_mechs.end())
   {
      throw std::runtime_error(mechName + " doesn't exists");
   }

   return it->second;
}

///////////////////////////////////////////////////////////////////////////////

RenderingMechanism* RenderingPipelineBuilder::end()
{
   // first - process all the dependencies to properly
   // initialize all the chunks that will be fed to the graph builder
   unsigned int depsCount = m_dependencies.size();
   for (unsigned int i = 0; i < depsCount; ++i)
   {
      const std::string& dependentMechanism = m_dependencies[i].first;
      const std::string& requiredMechanism = m_dependencies[i].second;

      MechsMap::iterator depMechIt = m_mechs.find(dependentMechanism);
      if (depMechIt == m_mechs.end())
      {
         throw std::runtime_error(std::string("Dependency error: mechanism ") + dependentMechanism + " isn't defined");
      }

      depMechIt->second.chunk->definePrerequisite(requiredMechanism);
   }

   // now feed the chunks to the graph builder
   for (MechsMap::iterator it = m_mechs.begin(); it != m_mechs.end(); ++it)
   {
      m_graphBuilder->addChunk(*(it->second.chunk));
   }

   // feed the rendering targets cleaner mechanism policy
   // with all the targets used by the pipeline
   unsigned int passIdx = 0;
   for (AllTargetsSet::iterator it = m_allTargets.begin(); 
        it != m_allTargets.end(); ++it, ++passIdx)
   {
      // assign each target to a separate pass in case
      // we're dealing with a low end system that can deal only with
      // one target at a time
      m_cleanerPolicy->addTarget(passIdx, **it);
   }
   m_allTargets.clear();
   m_cleanerPolicy = NULL;

   // and finally generate a composite rendering mechanism based on the graph
   RenderingMechanismBuilder builder;
   GraphBFS(m_renderGraph, 0, builder);
   return builder.get();
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

RenderingPipelineTransform::RenderingPipelineTransform(RenderingPipelineBuilder& parent,
                                                       const std::string& transformID)
      : m_parent(parent)
      , m_transformID(transformID)
      , m_inputsCount(0)
      , m_outputsCount(0)
{
   m_parent.createMechanism(transformID);
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineTransform& RenderingPipelineTransform::dep(const std::string& argID)
{
   m_parent.defineDependency(m_transformID, argID);

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineTransform& RenderingPipelineTransform::in(const std::string& argID)
{
   RenderingPipelineBuilder::MechDef& mech = m_parent.getMechDef(m_transformID);
   m_parent.assignSource(mech, argID);

   ++m_inputsCount;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineTransform& RenderingPipelineTransform::out(const std::string& argID, 
                                                            unsigned int passIdx)
{
   RenderingPipelineBuilder::MechDef& mech = m_parent.getMechDef(m_transformID);
   m_parent.addRenderTarget(*(mech.policy), passIdx, argID);

   ++m_outputsCount;

   return *this;
}

///////////////////////////////////////////////////////////////////////////////

RenderingPipelineBuilder& RenderingPipelineTransform::end()
{
   if (m_inputsCount == 0)
   {
      throw std::runtime_error(std::string("Mechanism ") + m_transformID 
                                    + " doesn't have any inputs defined");
   }

   if (m_outputsCount == 0)
   {
      throw std::runtime_error(std::string("Mechanism ") + m_transformID 
                                    + " doesn't have any outputs defined");
   }

   return m_parent;
}

///////////////////////////////////////////////////////////////////////////////
