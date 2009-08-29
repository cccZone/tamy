#include "core-TestFramework\TestFramework.h"
#include "core\Node.h"
#include "core\MatrixWriter.h"
#include <d3dx9.h>
#include "ext-MotionControllers\WaypointCameraController.h"
#include "ext-MotionControllers\TimeFunction.h"


///////////////////////////////////////////////////////////////////////////////

class LinearTimeFuncMock : public TimeFunction
{
public:
   float operator()(const float& advancement) const
   {
      return advancement;
   }
};

///////////////////////////////////////////////////////////////////////////////

TEST(WaypointCameraController, rotationsIncludedInMovementScheme)
{
   D3DXMATRIX expectedTrans;

   Node page("page");
   Node camera("camera");

   WaypointCameraController menuController(camera, D3DXVECTOR3(0, 0, -20), new LinearTimeFuncMock());

   menuController.registerWaypoint(0, page);

   D3DXMatrixRotationYawPitchRoll(&(page.accessLocalMtx()), D3DXToRadian(90), 0, 0);

   menuController.goTo(0);

   D3DXMATRIX helperMtx;
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(90), 0, 0);
   D3DXMatrixTranslation(&expectedTrans, -20, 0, 0);
   D3DXMatrixMultiply(&expectedTrans, &helperMtx, &expectedTrans);

   COMPARE_MTX(expectedTrans, menuController.getDestination());
}

///////////////////////////////////////////////////////////////////////////////

TEST(WaypointCameraController, complexMovementx)
{
   D3DXMATRIX expectedTrans;

   Node page("page");
   Node camera("camera");

   WaypointCameraController menuController(camera, D3DXVECTOR3(0, 10, -30), new LinearTimeFuncMock());

   menuController.registerWaypoint(0, page);

   D3DXMATRIX helperMtx;
   D3DXMatrixRotationYawPitchRoll(&helperMtx, D3DXToRadian(60), 0, 0);
   D3DXMatrixTranslation(&(page.accessLocalMtx()), 20, 5, 40);
   D3DXMatrixMultiply(&(page.accessLocalMtx()), &helperMtx, &(page.accessLocalMtx()));

   menuController.goTo(0);

   
   expectedTrans = D3DXMATRIX(0.5f,      0,        -0.866025f,  0,
                              0,         1,         0,          0,
                              0.866025f, 0,         0.5f,       0,
                              -5.980762, 14.99999f, 24.999998f, 1);

   COMPARE_MTX(expectedTrans, menuController.getDestination());
}

///////////////////////////////////////////////////////////////////////////////

TEST(WaypointCameraController, switchingBetweenPagesCanBeSpreadInTime)
{
   D3DXMATRIX expectedTrans;

   Node page0("page0");
   Node page1("page1");
   Node camera("camera");

   float dist = 10;
   float zOffset = -20;

   WaypointCameraController menuController(camera, D3DXVECTOR3(0, 0, zOffset), new LinearTimeFuncMock());

   menuController.registerWaypoint(0, page0);
   menuController.registerWaypoint(1, page1);
   D3DXMatrixTranslation(&(page1.accessLocalMtx()), 0, 0, dist);
   menuController.goTo(0);
   menuController.update(1);

   menuController.goTo(1);

   float totalTime = 0;
   float timeElapsed = 0;

   timeElapsed = 0.1f;
   totalTime += timeElapsed;
   menuController.update(timeElapsed);
   D3DXMatrixTranslation(&expectedTrans, 0, 0, totalTime * dist + zOffset);
   COMPARE_MTX(expectedTrans, camera.getLocalMtx());

   timeElapsed = 0.6f;
   totalTime += timeElapsed;
   menuController.update(timeElapsed);
   D3DXMatrixTranslation(&expectedTrans, 0, 0, totalTime * dist + zOffset);
   COMPARE_MTX(expectedTrans, camera.getLocalMtx());

   timeElapsed = 0.3f;
   totalTime += timeElapsed;
   menuController.update(timeElapsed);
   D3DXMatrixTranslation(&expectedTrans, 0, 0, totalTime * dist + zOffset);
   COMPARE_MTX(expectedTrans, camera.getLocalMtx());
}

///////////////////////////////////////////////////////////////////////////////
