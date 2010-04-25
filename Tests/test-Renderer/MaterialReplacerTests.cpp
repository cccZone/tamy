#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\MaterialReplacer.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\RenderableNode.h"


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

TEST(MaterialReplacer, replacingSelectedTechnique)
{
   Material material1("material1");
   Material material2("material2");
   RenderableNodeMock* graphNode1 = new RenderableNodeMock(material1);
   RenderableNodeMock* graphNode2 = new RenderableNodeMock(material2);
   RenderableNodeMock* graphNode3 = new RenderableNodeMock(material2);

   Node* compositeNode = new Node("compositeNode");
   compositeNode->addChild(graphNode1);
   compositeNode->addChild(graphNode2);

   Node rootNode("rootNode");
   rootNode.addChild(compositeNode);
   rootNode.addChild(graphNode3);
   
   // run the tests
   MaterialReplacer replacer(material2, material1);
   rootNode.accept(replacer);

   CPPUNIT_ASSERT_EQUAL(material1.getName(), graphNode1->getMaterial().getName());
   CPPUNIT_ASSERT_EQUAL(material1.getName(), graphNode2->getMaterial().getName());
   CPPUNIT_ASSERT_EQUAL(material1.getName(), graphNode3->getMaterial().getName());

}

///////////////////////////////////////////////////////////////////////////////
