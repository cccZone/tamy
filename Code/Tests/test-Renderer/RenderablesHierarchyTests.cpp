#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\SpatialEntity.h"


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{

   class SpatialEntityMock : public SpatialEntity
   {
      DECLARE_CLASS(SpatialEntityMock)

   public:
      SpatialEntityMock() : SpatialEntity("SpatialEntityMock") {}
   };
   BEGIN_OBJECT(SpatialEntityMock, SpatialEntity)
   END_OBJECT()

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(SpatialEntity, hierarchy)
{
   SpatialEntityMock parent;
   SpatialEntityMock* child = new SpatialEntityMock();
   parent.add(child);

   D3DXMATRIX expectedMtx;

   D3DXMatrixIdentity(&expectedMtx);
   COMPARE_MTX(expectedMtx, child->getGlobalMtx());

   D3DXMatrixTranslation(&expectedMtx, 10, 20, 30);
   parent.setLocalMtx(expectedMtx);
   COMPARE_MTX(expectedMtx, child->getGlobalMtx());
}

///////////////////////////////////////////////////////////////////////////////
