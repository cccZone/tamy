#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderLights.h"
#include "core-Renderer\LightsStorage.h"
#include "core-Renderer\Light.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\Material.h"
#include "core\BoundingSpace.h"
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

      void query(const BoundingVolume& volume, Array<Light*>& outLights) 
      {
         unsigned int lightsCount = m_lights.size();
         for (unsigned int i = 0; i < lightsCount; ++i)
         {
            outLights.push_back(m_lights[i]);
         }
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
