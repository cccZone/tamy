#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\GraphicalNode.h"
#include "core-Renderer\Material.h"
#include "LightReflectingPropertiesStub.h"
#include "TextureStub.h"
#include "core\MatrixWriter.h"
#include "GraphicalEntityMock.h"
#include "SkinnedGraphicalEntityMock.h"
#include <vector>
#include "NodeHierarchyWriter.h"
#include "core-Renderer\SkinnedGraphicalNode.h"
#include "core-Renderer\SkinBoneDefinition.h"


///////////////////////////////////////////////////////////////////////////////

TEST(SkinnedGraphicalNode, basics)
{
   std::vector<SkinBoneDefinition> skinBonesStub;
   std::vector<BonesInfluenceDefinition> boneSetsStub;
   boneSetsStub.push_back(std::vector<std::string>());
   std::vector<Material*> materialsStub;
   materialsStub.push_back(reinterpret_cast<Material*> (NULL));

   D3DXMATRIX pelvisOffsetMtx; D3DXMatrixIdentity(&pelvisOffsetMtx);
   D3DXMATRIX kneeOffsetMtx; D3DXMatrixTranslation(&kneeOffsetMtx, 5, 10, 0);
   D3DXMATRIX pelvisMtx; D3DXMatrixTranslation(&pelvisMtx, 9, 8, 7);
   D3DXMATRIX kneeMtx; D3DXMatrixTranslation(&kneeMtx, 1, 2, 3);

   std::vector<std::pair<Node*, D3DXMATRIX> > bones;
   Node pelvis("pelvis", false); pelvis.setLocalMtx(pelvisMtx);
   Node knee("knee", false); knee.setLocalMtx(kneeMtx);

   bones.push_back(std::make_pair(&pelvis, pelvisOffsetMtx));
   bones.push_back(std::make_pair(&knee, kneeOffsetMtx));

   SkinnedGraphicalEntityMock entity("legSkin", skinBonesStub, boneSetsStub, materialsStub);
   SkinnedGraphicalNode skin("legSkin", false, entity, 0, bones);

   // prepare results
   D3DXMATRIX resMtx;
   std::vector<D3DXMATRIX> expectedResults;
   D3DXMatrixMultiply(&resMtx, &pelvisOffsetMtx, &pelvisMtx); expectedResults.push_back(resMtx);
   D3DXMatrixMultiply(&resMtx, &kneeOffsetMtx, &kneeMtx); expectedResults.push_back(resMtx);


   // run the test
   skin.render();
  
   const std::vector<D3DXMATRIX>& results = entity.getMatricesUsedForRendering();
   CPPUNIT_ASSERT_EQUAL(expectedResults.size(), results.size());

   for (unsigned int i = 0; i < results.size(); ++i)
   {
      CPPUNIT_ASSERT_EQUAL(expectedResults[i], results[i]);
   }
}

///////////////////////////////////////////////////////////////////////////////
