#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderingTarget.h"
#include "core-Renderer\RenderablesStorage.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "core-Renderer\RenderingMechanismLeaf.h"
#include "core-Renderer\RendererImpl.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\RenderingGraph.h"
#include "core\GraphBFS.h"
#include "core-Renderer\CompositeRenderingMechanism.h"
#include "core-Renderer\RenderingMechanismBuilder.h"
#include "core-Renderer\RenderingPipelineDefinition.h"
#include "core-Renderer\RenderingPipelineBuilder.h"
#include "core-Renderer\RenderingTargetsCleaner.h"
#include "core-Renderer\TargetsCleaningMechanism.h"
#include "core\GraphBuilder.h"
#include "RendererImplementationMock.h"
#include <vector>
#include <set>
#include <map>
#include <list>
#include <algorithm>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RenderingMechanismMock : public RenderingMechanism
   {
   private:
      std::string m_id;
      std::vector<std::string>& m_callSeq;

   public:
      RenderingMechanismMock(const std::string& id, 
         std::vector<std::string>& callSeq)
         : m_id(id)
         , m_callSeq(callSeq)
      {}

      void render()
      {
         m_callSeq.push_back(m_id + " rendering");
      }
   };

   // -------------------------------------------------------------------------

   class GraphRenderOperationMock
   {
   public:
      void operator()(const RenderingNode& node)
      {
         if (node.mech != NULL)
         {
            node.mech->render();
         }
      }
   };

   // ------------------------------------------------------------------------

   class RenderableStub : public Renderable
   {
   private:
      Array<D3DXMATRIX> m_renderingMatrices;
      D3DXMATRIX m_mtx;
      Material m_mat;

   public:
      RenderableStub() : m_mat("matStub") {}
      
      void render() {}

      const Array<D3DXMATRIX>& getRenderingMatrices() {return m_renderingMatrices;}
      
      const Material& getMaterial() const {return m_mat;}

      const D3DXMATRIX& getGlobalTransform() {return m_mtx;}
   };

   // -------------------------------------------------------------------------

   class RenderingTargetsCleanerMock : public RenderingTargetsCleaner
   {
   private:
      std::vector<std::string>& m_callSeq;

   public:
      RenderingTargetsCleanerMock(std::vector<std::string>& callSeq)
         : m_callSeq(callSeq)
      {}

      void cleanRenderingTargets(unsigned int targetsCount) 
      {
         m_callSeq.push_back("cleaningTargets");
      }
   };

   // ------------------------------------------------------------------------

   class RenderingPipelineDefinitionMock : public RenderingPipelineDefinition
   {
   private:
      std::vector<std::string>& m_callSeq;

      typedef std::map<std::string, RenderingTarget*> TargetsSet;
      TargetsSet m_targets;

      RegularTests::RendererImplementationMock m_renderer;
      Camera m_camera;

   public:
      RenderingPipelineDefinitionMock(std::vector<std::string>& callSeq)
         : m_callSeq(callSeq)
         , m_camera("cameraStub", m_renderer)
      {}

      ~RenderingPipelineDefinitionMock()
      {
         for (TargetsSet::iterator it = m_targets.begin(); 
              it != m_targets.end(); ++it)
         {
            delete it->second;
         }
         m_targets.clear();
      }

      void createMechanism(const std::string& mechName, 
                           SettableRenderingTargetsPolicy* policy,
                           RenderingMechanismLeaf** outMechanism)
      {
         class RendererImplMock : public RendererImpl
         {
         private:
            std::string m_id;
            std::vector<std::string>& m_callSeq;

         public:
            RendererImplMock(const std::string& id,
                             std::vector<std::string>& callSeq) 
               : m_id(id)
               , m_callSeq(callSeq) 
            {}

            void render(Renderable& renderable, RenderingTargetsPolicy& policy) 
            {
               policy.setTargets(0);
               m_callSeq.push_back(std::string("rendering: ") + m_id);
            }

            void cleanRenderingTargets(unsigned int targetsCount) {}

            void enableLights(const Array<Light*>& lights) {}

            void disableLights(const Array<Light*>& lights) {}

            void setMatrices(const D3DXMATRIX& viewMtx, const D3DXMATRIX& projMtx) {}

            void setObjectMatrices(const Array<D3DXMATRIX>& matrices) {}

            void setTransparency(bool enable) {}
            
            void setLRP(LightReflectingProperties& lrp) {}

            void setTexture(unsigned char stageIndex, Texture& textureImpl) {}

            void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp) {}

            void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp) {}

            void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp) {}
         };

         *outMechanism = new RenderingMechanismLeaf(policy, 
                                                    new RendererImplMock(mechName, m_callSeq));
         (*outMechanism)->setActiveCamera(m_camera);
      }

      RenderingMechanism* createTargetsCleaner(RenderingTargetsPolicy* policy)
      {
         return new TargetsCleaningMechanism(policy,
                                             new RenderingTargetsCleanerMock(m_callSeq));
      }
      
      RenderablesStorage* createStorage(const std::string& sourceName)
      {
         class RenderablesStorageMock : public RenderablesStorage
         {
         private:
            std::string m_id;
            std::vector<std::string>& m_callSeq;
            RenderableStub m_renderable;

         public:
            RenderablesStorageMock(const std::string& id,
               std::vector<std::string>& callSeq) 
               : m_id(id)
               , m_callSeq(callSeq)
            {}

            void query(Camera& camera, Array<Renderable*>& outNodes)
            {
               m_callSeq.push_back(std::string("using source: ") + m_id);

               outNodes.push_back(&m_renderable);
            }
         };

         return new RenderablesStorageMock(sourceName, m_callSeq);
      }

      RenderingTarget& getRenderTarget(const std::string& targetName)
      {
         class RenderingTargetMock : public RenderingTarget
         {
         private:
            std::string m_id;
            std::vector<std::string>& m_callSeq;

         public:
            RenderingTargetMock(const std::string& id,
               std::vector<std::string>& callSeq) 
               : m_id(id)
               , m_callSeq(callSeq){}

            void use(unsigned char idx) 
            {
               m_callSeq.push_back(std::string("using target: ") + m_id);
            }
         };
      
         RenderingTarget* target = NULL;

         TargetsSet::iterator it = m_targets.find(targetName);
         if (it != m_targets.end())
         {
            target = it->second;
         }
         else
         {
            target = new RenderingTargetMock(targetName, m_callSeq);
            m_targets.insert(std::make_pair(targetName, target));
         }

         return *target;
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingGraph, simpleGraph)
{
   std::vector<std::string> callSeq;
   RenderingMechanismMock* mech1 = new RenderingMechanismMock("mech1", callSeq);
   RenderingMechanismMock* mech2 = new RenderingMechanismMock("mech2", callSeq);

   RenderingGraph renderGraph;
   renderGraph.addNode(RenderingNode(mech1));
   renderGraph.addNode(RenderingNode(mech2));
   renderGraph.connect(0, RenderingEdge(1));

   GraphRenderOperationMock op;
   GraphBFS(renderGraph, 0, op);

   delete mech1;
   delete mech2;

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callSeq.size());
   CPPUNIT_ASSERT_EQUAL(std::string("mech1 rendering"), callSeq[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech2 rendering"), callSeq[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingMechanismBuilder, twoParallelMechanisms)
{
   std::vector<std::string> callSeq;

   RenderingGraph renderGraph;
   renderGraph.addNode(RenderingNode());
   renderGraph.addNode(RenderingNode(new RenderingMechanismMock("mech1", callSeq)));
   renderGraph.addNode(RenderingNode(new RenderingMechanismMock("mech2", callSeq)));
   renderGraph.connect(0, RenderingEdge(1));
   renderGraph.connect(0, RenderingEdge(2));

   RenderingMechanismBuilder builder;
   GraphBFS(renderGraph, 0, builder);

   RenderingMechanism* mechanism = builder.get();
   mechanism->render();
   delete mechanism;

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, callSeq.size());
   CPPUNIT_ASSERT_EQUAL(std::string("mech1 rendering"), callSeq[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech2 rendering"), callSeq[1]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingPipelineBuilder, simpleHierarchy)
{
   std::vector<std::string> callSeq;

   RenderingPipelineDefinitionMock definition(callSeq);
   RenderingPipelineBuilder builder(definition);

   RenderingMechanism* mechanism = builder
      .defineTransform("effect1")
         .in("dynamicScene1")
         .out("target1")
         .out("target2")
         .end()
      .defineTransform("effect2")
         .in("dynamicScene2")
         .dep("effect1")
         .out("screen")
         .end()
   .end();

   CPPUNIT_ASSERT(NULL != mechanism);

   mechanism->render();
   delete mechanism;

   CPPUNIT_ASSERT_EQUAL((unsigned int)13, callSeq.size());

   // calls 0 - 5 are cleaning calls (unordered due to the impl details)

   CPPUNIT_ASSERT_EQUAL(std::string("using source: dynamicScene1"), callSeq[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target1"), callSeq[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target2"), callSeq[8]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: effect1"), callSeq[9]);
   CPPUNIT_ASSERT_EQUAL(std::string("using source: dynamicScene2"), callSeq[10]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: screen"), callSeq[11]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: effect2"), callSeq[12]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingPipelineBuilder, outputMustBeUdpatedByAllInterestedPertiesBeforeItCanBeUsedAsSource)
{
   std::vector<std::string> callSeq;

   RenderingPipelineDefinitionMock definition(callSeq);
   RenderingPipelineBuilder builder(definition);

   // this time we define them backwards
   RenderingMechanism* mechanism = builder
      .defineTransform("combine")
         .dep("opaque")
         .dep("transparent")
         .in("renderedScene")
         .out("screen")
         .end()
      .defineTransform("transparent")
         .in("scene")
         .out("target")
         .end()
      .defineTransform("opaque")
         .in("scene")
         .out("target")
         .end()
   .end();

   CPPUNIT_ASSERT(NULL != mechanism);

   mechanism->render();
   delete mechanism;

   CPPUNIT_ASSERT_EQUAL((unsigned int)13, callSeq.size());

   // calls 0 - 3 are cleaning calls (unordered due to the impl details)

   CPPUNIT_ASSERT_EQUAL(std::string("using source: scene"), callSeq[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target"), callSeq[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: opaque"), callSeq[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("using source: scene"), callSeq[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target"), callSeq[8]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: transparent"), callSeq[9]);
   CPPUNIT_ASSERT_EQUAL(std::string("using source: renderedScene"), callSeq[10]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: screen"), callSeq[11]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: combine"), callSeq[12]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingPipelineBuilder, complexPipeline)
{
   std::vector<std::string> callSeq;

   RenderingPipelineDefinitionMock definition(callSeq);
   RenderingPipelineBuilder builder(definition);

   RenderingMechanism* mechanism = builder
      .defineTransform("opaque1")
         .in("scene1")
         .out("target1")
         .end()
      .defineTransform("transparent1")
         .dep("opaque1")
         .in("scene1")
         .out("target1")
         .end()
      .defineTransform("postProcess1")
         .dep("transparent1")
         .in("renderedScene1")
         .out("target2")
         .end()
      .defineTransform("opaque2")
         .in("scene2")
         .out("target3")
         .end()
      .defineTransform("transparent2")
         .dep("opaque2")
         .in("scene2")
         .out("target3")
         .end()
      .defineTransform("postProcess2")
         .dep("transparent2")
         .in("renderedScene2")
         .out("target4")
         .end()
      .defineTransform("combine")
         .dep("postProcess1")
         .dep("postProcess2")
         .in("postProcessTargets")
         .out("screen")
         .end()
   .end();

   CPPUNIT_ASSERT(NULL != mechanism);

   mechanism->render();
   delete mechanism;

   CPPUNIT_ASSERT_EQUAL((unsigned int)31, callSeq.size());

   // calls 0 - 9 are cleaning calls (unordered due to the impl details)

   CPPUNIT_ASSERT_EQUAL(std::string("using source: scene1"), callSeq[10]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target1"), callSeq[11]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: opaque1"), callSeq[12]);

   CPPUNIT_ASSERT_EQUAL(std::string("using source: scene2"), callSeq[13]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target3"), callSeq[14]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: opaque2"), callSeq[15]);

   CPPUNIT_ASSERT_EQUAL(std::string("using source: scene1"), callSeq[16]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target1"), callSeq[17]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: transparent1"), callSeq[18]);

   CPPUNIT_ASSERT_EQUAL(std::string("using source: scene2"), callSeq[19]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target3"), callSeq[20]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: transparent2"), callSeq[21]);

   CPPUNIT_ASSERT_EQUAL(std::string("using source: renderedScene1"), callSeq[22]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target2"), callSeq[23]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: postProcess1"), callSeq[24]);

   CPPUNIT_ASSERT_EQUAL(std::string("using source: renderedScene2"), callSeq[25]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: target4"), callSeq[26]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: postProcess2"), callSeq[27]);

   CPPUNIT_ASSERT_EQUAL(std::string("using source: postProcessTargets"), callSeq[28]);
   CPPUNIT_ASSERT_EQUAL(std::string("using target: screen"), callSeq[29]);
   CPPUNIT_ASSERT_EQUAL(std::string("rendering: combine"), callSeq[30]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingPipelineBuilder, missingOutput)
{
   std::vector<std::string> callSeq;

   RenderingPipelineDefinitionMock definition(callSeq);
   RenderingPipelineBuilder builder(definition);

   CPPUNIT_ASSERT_THROW(
      (builder
         .defineTransform("something")
            .in("target")
            .end()
      .end()),
      std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingPipelineBuilder, missingInput)
{
   std::vector<std::string> callSeq;

   RenderingPipelineDefinitionMock definition(callSeq);
   RenderingPipelineBuilder builder(definition);

   CPPUNIT_ASSERT_THROW(
      (builder
         .defineTransform("something")
            .out("target")
            .end()
      .end()),
      std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderingPipelineBuilder, sameMechanismUsedTwice)
{
   std::vector<std::string> callSeq;

   RenderingPipelineDefinitionMock definition(callSeq);
   RenderingPipelineBuilder builder(definition);

   CPPUNIT_ASSERT_THROW((builder
                     .defineTransform("defaultRenderer")
                        .in("scene1")
                        .out("target1")
                        .end()
                     .defineTransform("defaultRenderer")
                        .in("scene2")
                        .out("target2")
                        .end()
                        .end()), std::runtime_error);
}

///////////////////////////////////////////////////////////////////////////////
