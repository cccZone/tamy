#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\StaticGeometryRenderable.h"
#include "core-Renderer\LitVertex.h"
#include "core-Renderer\Face.h"
#include "core-Renderer\Material.h"
#include <vector>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   
   class StaticGeometryRenderableMock : public StaticGeometryRenderable
   {
   public:
      StaticGeometryRenderableMock(Material& material,
                                   const std::vector<LitVertex>& vertices,
                                   const std::vector<Face<USHORT> >& faces) 
         : StaticGeometryRenderable(material, vertices, faces)
      {}

      void render()
      {
      }

   protected:
      StaticGeometryRenderable* createSelf(Material& material,
                                           const std::vector<LitVertex>& vertices,
                                           const std::vector<Face<USHORT> >& faces)
      {
         return new StaticGeometryRenderableMock(material, vertices, faces);
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryRenderable, creatingNewObjects)
{
   Material material("material");

   std::vector<LitVertex> vertices;
   vertices.push_back(LitVertex::unskinnedNoTex(0, 0, 0, 0, 1, 0));
   vertices.push_back(LitVertex::unskinnedNoTex(1, 0, 0, 0, 1, 0));
   vertices.push_back(LitVertex::unskinnedNoTex(1, 0, 1, 0, 1, 0));
   std::vector<Face<USHORT> > faces;
   
   StaticGeometryRenderableMock renderable(material, vertices, faces);
   const AABoundingBox& vol = renderable.getBoundingVolume();
   COMPARE_VEC(D3DXVECTOR3(0, 0, 0), vol.min);
   COMPARE_VEC(D3DXVECTOR3(1, 0, 1), vol.max);
}

///////////////////////////////////////////////////////////////////////////////

TEST(StaticGeometryRenderable, geometrySplittingBasics)
{
   Material material("material");
   std::vector<LitVertex> vertices;
   std::vector<Face<USHORT> > faces;

   vertices.push_back(LitVertex::unskinnedNoTex(-1, 0, 0, 0, 1, 0));
   vertices.push_back(LitVertex::unskinnedNoTex( 1, 0, 0, 0, 1, 0));
   vertices.push_back(LitVertex::unskinnedNoTex( 0, 0, 1, 0, 1, 0));
   faces.push_back(Face<USHORT>(0, 1, 2, 0));
   StaticGeometryRenderableMock renderable1(material, vertices, faces);

   StaticGeometryRenderable* frontRenderable = NULL;
   StaticGeometryRenderable* backRenderable = NULL;
   renderable1.split(D3DXPLANE(1, 0, 0, 0), &frontRenderable, &backRenderable);

   CPPUNIT_ASSERT(NULL != frontRenderable);
   CPPUNIT_ASSERT(NULL != backRenderable);

   CPPUNIT_ASSERT_EQUAL((unsigned int)3, frontRenderable->getVertices().size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, backRenderable->getVertices().size());

   delete frontRenderable;
   delete backRenderable;
}

///////////////////////////////////////////////////////////////////////////////
