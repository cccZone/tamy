#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\SkyBoxSide.h"
#include "core-Renderer\SkyBoxStorage.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\Texture.h"
#include "RendererImplementationMock.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class TextureStub : public Texture
   {
   public:
      TextureStub() : Texture("textureStub") {}

      TextureImpl& getImpl() {return *(reinterpret_cast<TextureImpl*>(NULL));}
   };

   // -------------------------------------------------------------------------

   class SkyBoxSideMock : public SkyBoxSide
   {
   public:
      SkyBoxSideMock(Texture* texture) : SkyBoxSide(texture) {}

      void render() {}
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(SkyBoxSide, alteringMatrices)
{
   SkyBoxSideMock side(new TextureStub);

   D3DXMATRIX transformMtx;
   D3DXMatrixTranslation(&transformMtx, 10, -20, 30);

   D3DXMATRIX identityMtx;
   D3DXMatrixIdentity(&identityMtx);

   side.setGlobalMtx(transformMtx);

   // global matrix doesn't change - the skybox always 
   // is centered at the origin of the world-space coordinate system
   COMPARE_MTX(identityMtx, side.getGlobalTransform());

   // rendering matrices do change - in order for the skybox
   // to be rendered wherever the camera is
   const Array<D3DXMATRIX>& renderingMtcs = side.getRenderingMatrices();
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, renderingMtcs.size());
   COMPARE_MTX(transformMtx, renderingMtcs[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(SkyBoxStorage, cameraPositionInflunecesReturnedRenderables)
{
   SkyBoxSideMock* front = new SkyBoxSideMock(new TextureStub);
   SkyBoxSideMock* back = new SkyBoxSideMock(new TextureStub);
   SkyBoxSideMock* left = new SkyBoxSideMock(new TextureStub);
   SkyBoxSideMock* right = new SkyBoxSideMock(new TextureStub);
   SkyBoxSideMock* top = new SkyBoxSideMock(new TextureStub);
   SkyBoxSideMock* bottom = new SkyBoxSideMock(new TextureStub);

   SkyBoxStorage storage(left, right, top, bottom, front, back);

   RegularTests::RendererImplementationMock renderer;
   Camera camera("cam", renderer);

   D3DXMATRIX transformMtx;
   D3DXMATRIX rotMtx;
   D3DXMatrixRotationYawPitchRoll(&rotMtx, D3DXToRadian(90), 0, 0);
   D3DXMatrixTranslation(&transformMtx, 10, 0, 0);
   D3DXMatrixMultiply(&transformMtx, &transformMtx, &rotMtx); // rotating the translated point
   camera.setLocalMtx(transformMtx);

   D3DXMATRIX expectedTrans;
   D3DXMatrixTranslation(&expectedTrans, 0, 0, -10);
   
   Array<Renderable*> renderables;
   storage.query(camera, renderables);
   CPPUNIT_ASSERT_EQUAL((unsigned int)6, renderables.size());

   COMPARE_MTX(expectedTrans, renderables[0]->getRenderingMatrices()[0]);
   COMPARE_MTX(expectedTrans, renderables[1]->getRenderingMatrices()[0]);
   COMPARE_MTX(expectedTrans, renderables[2]->getRenderingMatrices()[0]);
   COMPARE_MTX(expectedTrans, renderables[3]->getRenderingMatrices()[0]);
   COMPARE_MTX(expectedTrans, renderables[4]->getRenderingMatrices()[0]);
   COMPARE_MTX(expectedTrans, renderables[5]->getRenderingMatrices()[0]);
}

///////////////////////////////////////////////////////////////////////////////
