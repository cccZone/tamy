#include "core-TestFramework\TestFramework.h"
#include "core-MVC\Model.h"
#include "core-MVC\ModelView.h"
#include "core-Renderer\SpatialView.h"
#include "core-Renderer\CameraContext.h"
#include "core-Renderer\Renderable.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core\BoundingSphere.h"
#include "core\AABoundingBox.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class GeometryMock : public GeometryResource
   {
   private:
      BoundingVolume* m_vol;

   public:
      GeometryMock() : m_vol(NULL) {}
      GeometryMock(BoundingVolume* vol) : m_vol(vol) {}
      ~GeometryMock() {delete m_vol; m_vol = NULL;}

      void render() {}

      BoundingVolume* calculateBoundingVolume() const
      {
         D3DXMATRIX idMtx;
         D3DXMatrixIdentity(&idMtx);
         return *m_vol * idMtx;
      }

      void onResourceLoaded(ResourcesManager& mgr) {}
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(SpatialView, regularOctreeSpatialView)
{
   Model scene3D;

   AABoundingBox treeBB(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
   SpatialView view(treeBB);
   scene3D.attach(view);

   GeometryMock geometry(new BoundingSphere(D3DXVECTOR3(0, 0, 0), 1));

   Renderable* entity1 = new Renderable();
   Renderable* entity2 = new Renderable();
   entity1->add(new Geometry(geometry));
   entity1->setPosition(D3DXVECTOR3(-5, -5, -5));
   entity2->add(new Geometry(geometry));
   entity2->setPosition(D3DXVECTOR3(5, 5, 5));
   scene3D.add(entity1);
   scene3D.add(entity2);

   TCameraContext<BoundingSphere> camera(BoundingSphere(D3DXVECTOR3(5, 5, 5), 2));
   view.update(camera);
   CPPUNIT_ASSERT_EQUAL(false, entity1->isVisible());
   CPPUNIT_ASSERT_EQUAL(true, entity2->isVisible());
}

///////////////////////////////////////////////////////////////////////////////

