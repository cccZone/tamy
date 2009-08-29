#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderLights.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\LightsSortingStorage.h"
#include "core-Renderer\LightsSorter.h"
#include "RendererImplMock.h"


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{

   class RenderableNodeStub : public RenderableNode
   {
   private:
      static Material s_material;
      Array<D3DXMATRIX> m_matrices;

   public:
      RenderableNodeStub()
            : RenderableNode("RenderableNodeMock", s_material)
      {}

      void render() {}

      const Array<D3DXMATRIX>& getRenderingMatrices() {return m_matrices;}

   };

   Material RenderableNodeStub::s_material("materialStub");

   // -------------------------------------------------------------------------

   class SimpleLightsStorage : public LightsStorage
   {
   private:
      std::vector<Light*> m_lights;

   public:
      void add(Light& light) {m_lights.push_back(&light);}

      void query(const D3DXVECTOR3& point, Array<Light*>& outLights) 
      {
         unsigned int lightsCount = m_lights.size();
         for (unsigned int i = 0; i < lightsCount; ++i)
         {
            outLights.push_back(m_lights[i]);
         }
      }
   };

   // -------------------------------------------------------------------------

   class LightsSorterMock
   {
   private:
      bool m_used;

   public:
      LightsSorterMock() 
         : m_used(false) 
      {
      }

      void setInfluencedPos(const D3DXVECTOR3& pos)
      {
         m_used = true;
      }

      bool operator()(Light* lhs, Light* rhs) const
      {
         return (DWORD)lhs < (DWORD)rhs;
      }

      bool wasUsed() const
      {
         return m_used;
      }
   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(RenderLights, lightsEnabledAreDisabledLaterOn)
{
   std::vector<std::string> results;
   CallSeqTrack::RendererImplMock rendererImpl(results);

   RenderableNodeStub node;

   Light light1("light1");
   Light light2("light2");
   SimpleLightsStorage storage;
   storage.add(light1);
   storage.add(light2);

   RenderLights lightsRenderer(storage);

   // checking the behavior on rendering start
   lightsRenderer.onRenderingBegin(node, rendererImpl);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Enabling lights: light1, light2"), results[0]);

   results.clear();

   // checking the behavior on rendering end
   lightsRenderer.onRenderingEnd(node, rendererImpl);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Disabling lights: light1, light2"), results[0]);

}

///////////////////////////////////////////////////////////////////////////////

TEST(LightsSorter, pointLights)
{
   Light light1("light1");
   Light light2("light2");

   D3DXMatrixTranslation(&(light1.accessLocalMtx()), 10, 0, 0);
   D3DXMatrixTranslation(&(light1.accessLocalMtx()), -10, 0, 0);
   light1.setRange(15);
   light2.setRange(15);

   LightsSorter sorter;
   CPPUNIT_ASSERT_EQUAL(false, sorter(&light1, &light2)); // equal influence over the point
}

///////////////////////////////////////////////////////////////////////////////

TEST(LightsSortingStorage, lightsSortedOnline)
{   
   Light light1("light1");
   Light light2("light2");
   SimpleLightsStorage* baseStorage = new SimpleLightsStorage();
   baseStorage->add(light1);
   baseStorage->add(light2);

   LightsSorterMock* sorter = new LightsSorterMock();
   LightsSortingStorage<LightsSorterMock> storage(baseStorage, sorter);
   
   Array<Light*> outLights;

   baseStorage->query(D3DXVECTOR3(0, 0, 0), outLights);
   CPPUNIT_ASSERT_EQUAL(false, sorter->wasUsed());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, outLights.size());

   outLights.clear();
   storage.query(D3DXVECTOR3(0, 0, 0), outLights);
   CPPUNIT_ASSERT_EQUAL(true, sorter->wasUsed());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, outLights.size());
}

///////////////////////////////////////////////////////////////////////////////
