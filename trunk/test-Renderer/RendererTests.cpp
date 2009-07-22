#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\VisualSceneManager.h"
#include "RendererImplementationMock.h"
#include "RenderingMechanismMock.h"
#include "RenderingTargetsPolicyMock.h"
#include "core-Renderer\SettableRenderingTargetsPolicy.h"
#include "RenderingTargetMock.h"


using namespace RendererTests;

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingOnlyWhenRenderingDeviceIsReady)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);
   renderer.addMechanism(new RenderingMechanismMock("mech0", 
                                                    callSequence,
                                                    new RenderingTargetsPolicyMock("policy0", callSequence)));

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   renderer.setDeviceReady(false);

   callSequence.clear();
   renderer.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, callSequence.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, recoveringTheLostDevice)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);
   renderer.addMechanism(new RenderingMechanismMock("mech0", 
                                                    callSequence,
                                                    new RenderingTargetsPolicyMock("policy0", callSequence)));

   // initial rendering
   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   // we loose the device
   renderer.setDeviceReady(false);
   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, callSequence.size()); // we don't attempt to recover the device just yet - let's give it a loop

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - attemptToRecoverGraphicsSystem"), callSequence[0]);

   // the RendererImplementationMock will consider every recovery attempt a success, so the next
   // rendering pass should proceed as if the device was ready

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[2]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, noRenderingWithoutMechanisms)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);

   // initial rendering
   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, callSequence.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, multipleRenderingMechanisms)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);
   renderer.addMechanism(new RenderingMechanismMock("mech0", 
                                                    callSequence, 
                                                    new RenderingTargetsPolicyMock("policy0", callSequence)));
   renderer.addMechanism(new RenderingMechanismMock("mech1", 
                                                    callSequence, 
                                                    new RenderingTargetsPolicyMock("policy0", callSequence)));
   renderer.addMechanism(new RenderingMechanismMock("mech2", 
                                                    callSequence, 
                                                    new RenderingTargetsPolicyMock("policy0", callSequence)));

   // initial rendering
   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)10, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);

   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech1 - render"), callSequence[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[6]);

   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech2 - render"), callSequence[8]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[9]);

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)9, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[2]);

   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech1 - render"), callSequence[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[5]);

   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech2 - render"), callSequence[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[8]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, mechanismSetsUpItsOwnRenderTargetPolicy)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);
   RenderingMechanismMock* mechanism0 = new RenderingMechanismMock("mech0", 
                                                                   callSequence, 
                                                                   new RenderingTargetsPolicyMock("policy0", callSequence));
   renderer.addMechanism(mechanism0);
   renderer.render();
   callSequence.clear();

   renderer.getRenderingTargetsPolicy().setTargets(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("policy0 - setTargets"), callSequence[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, allRenderTargetsAreCleanedBeforeRendering)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);

   SettableRenderingTargetsPolicy* policy = new SettableRenderingTargetsPolicy();
   RenderingMechanismMock* mechanism0 = new RenderingMechanismMock("mech0", 
                                                                   callSequence, 
                                                                   policy);
   renderer.addMechanism(mechanism0);

   RendereringPipelineTests::RenderingTargetMock target0("target0", callSequence);
   RendereringPipelineTests::RenderingTargetMock target1("target1", callSequence);
   policy->addTarget(0, target0);
   policy->addTarget(1, target1);
   policy->addTarget(1, target0);

   renderer.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)9, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("target0 - 0"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - cleanAllTargets"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("target1 - 0"), callSequence[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("target0 - 1"), callSequence[4]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - cleanAllTargets"), callSequence[5]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[6]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[7]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[8]);
}

///////////////////////////////////////////////////////////////////////////////
