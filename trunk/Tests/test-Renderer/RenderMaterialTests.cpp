#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\RenderMaterial.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\RenderableNode.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "RendererImplMock.h"


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{

   class RenderableNodeMock : public RenderableNode
   {
   private:
      Array<D3DXMATRIX> m_matrices;

   public:
      RenderableNodeMock(Material& material)
            : RenderableNode("RenderableNodeMock", material)
      {}

      void render() {}

      const Array<D3DXMATRIX>& getRenderingMatrices() {return m_matrices;}

   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(RenderMaterial, materialNotSetIfItDoesntChange)
{
   RenderMaterial::resetMaterialMemory();

   // create the node we'll use for rendering
   Material material1("material1"); material1.getLightReflectingProperties().setPower(2);
   Material material2("material2"); material2.getLightReflectingProperties().setPower(3);
   RenderableNodeMock node1(material1);
   RenderableNodeMock node2(material1);
   RenderableNodeMock node3(material2);
   RenderableNodeMock node4(material1);

   std::vector<std::string> results;
   CallSeqTrack::RendererImplMock rendererImpl(results);
   RenderMaterial matRenderer;

   matRenderer.onRenderingBegin(node1, rendererImpl);
   matRenderer.onRenderingBegin(node2, rendererImpl);
   matRenderer.onRenderingBegin(node3, rendererImpl);
   matRenderer.onRenderingBegin(node4, rendererImpl);

   // compare the results
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 2.0"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 3.0"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 2.0"), results[2]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(RenderMaterial, materialSetPerMechanism)
{
   RenderMaterial::resetMaterialMemory();

   // create the node we'll use for rendering
   Material material1("material1"); material1.getLightReflectingProperties().setPower(2);
   Material material2("material2"); material2.getLightReflectingProperties().setPower(3);
   RenderableNodeMock node1(material1);
   RenderableNodeMock node2(material2);

   std::vector<std::string> results;
   CallSeqTrack::RendererImplMock rendererImpl(results);
   RenderMaterial matRenderer1;
   RenderMaterial matRenderer2;

   // 1st rendering cycle
   matRenderer1.onRenderingBegin(node1, rendererImpl);
   matRenderer2.onRenderingBegin(node2, rendererImpl);

   // 2nd rendering cycle
   matRenderer1.onRenderingBegin(node1, rendererImpl);
   matRenderer2.onRenderingBegin(node2, rendererImpl);

   // compare the results
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, results.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 2.0"), results[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 3.0"), results[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 2.0"), results[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Set material with power = 3.0"), results[3]);
}

///////////////////////////////////////////////////////////////////////////////
