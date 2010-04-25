#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\GeometryRenderingMechanism.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\RenderingAction.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\RenderingEffectRenderer.h"
#include "core-Renderer\RenderablesStorage.h"
#include "core-Renderer\Camera.h"
#include "RendererImplMock.h"
#include "RenderingTargetsPolicyMock.h"
#include "GraphicalEffectMock.h"
#include "RendererImplementationMock.h"


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{

   class RenderableNodeMock : public RenderableNode
   {
   private:
      static Material s_material;
      Array<D3DXMATRIX> m_matrices;
      std::vector<std::string>& m_callLog;

   public:
      RenderableNodeMock(const std::string& id, std::vector<std::string>& callLog)
            : RenderableNode(id, s_material),
            m_callLog(callLog)
      {}

      void render() 
      {
         m_callLog.push_back(std::string("Render node ") + getName());
      }

      const Array<D3DXMATRIX>& getRenderingMatrices() {return m_matrices;}

   };

   Material RenderableNodeMock::s_material("materialStub");

   // -------------------------------------------------------------------------

   class RenderablesStorageMock : public RenderablesStorage
   {
   private:
      std::vector<Renderable*> m_objects;

   public:
      void add(Renderable& object) {m_objects.push_back(&object);}

      void query(Camera& camera, Array<Renderable*>& output) 
      {
         unsigned int objectsCount = m_objects.size();
         for (unsigned int i = 0; i < objectsCount; ++i)
         {
            output.push_back(m_objects[i]);
         }
      }

   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GeometryRenderingMechanism, renderingSingleObject)
{
   std::vector<std::string> results;
   D3DXMATRIX sampleViewMtx, sampleProjMtx;
   GeometryRenderingMechanism processor(new CallSeqTrack::RenderingTargetsPolicyMock(results), 
                                    new CallSeqTrack::RendererImplMock(results));

   // create the node we'll use for rendering
   RenderableNodeMock node("node1", results);

   RenderablesStorageMock* storage = new RenderablesStorageMock();
   storage->add(node);
   processor.setStorage(storage);

   // set the default camera
   RegularTests::RendererImplementationMock renderer;
   Camera cam("cam", renderer);
   processor.setActiveCamera(cam);

   // run the processor
   processor.render();

   // compare the results
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Set view & proj matrices"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Setting object matrices"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Rendering a renderable..."), results[2]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(GeometryRenderingMechanism, renderingMultipleObjects)
{
   std::vector<std::string> results;
   D3DXMATRIX sampleViewMtx, sampleProjMtx;
   GeometryRenderingMechanism processor(new CallSeqTrack::RenderingTargetsPolicyMock(results), 
                                    new CallSeqTrack::RendererImplMock(results));

   // create the node we'll use for rendering
   RenderableNodeMock node1("node1", results);
   RenderableNodeMock node2("node2", results);

   RenderablesStorageMock* storage = new RenderablesStorageMock();
   storage->add(node1);
   storage->add(node2);
   processor.setStorage(storage);

   // set the default camera
   RegularTests::RendererImplementationMock renderer;
   Camera cam("cam", renderer);
   processor.setActiveCamera(cam);

   // run the processor
   processor.render();

   // compare the results
   CPPUNIT_ASSERT_EQUAL((unsigned int)5, results.size());

   // general setup
   CPPUNIT_ASSERT_EQUAL(std::string("Set view & proj matrices"), results[0]);

   // object 1 is being rendered
   CPPUNIT_ASSERT_EQUAL(std::string("Setting object matrices"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Rendering a renderable..."), results[2]);

   // object 2 is being rendered
   CPPUNIT_ASSERT_EQUAL(std::string("Setting object matrices") , results[3]);
   CPPUNIT_ASSERT_EQUAL(std::string("Rendering a renderable..."), results[4]);

}

///////////////////////////////////////////////////////////////////////////////
