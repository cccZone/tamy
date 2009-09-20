#include "core-TestFramework\TestFramework.h"
#include "core\MatrixWriter.h"
#include <d3dx9.h>
#include "core\MatrixInterpolator.h"


///////////////////////////////////////////////////////////////////////////////

TEST(MatrixInterpolator, translation)
{
   D3DXMATRIX start;
   D3DXMATRIX end;
   D3DXMATRIX expectedResult;

   D3DXMatrixTranslation(&start, 0, 0, 10);
   D3DXMatrixTranslation(&end, 0, 0, 20);

   MatrixInterpolator interpolator;

   D3DXMatrixTranslation(&expectedResult, 0, 0, 10);
   COMPARE_MTX(expectedResult, interpolator(start, end, 0));

   D3DXMatrixTranslation(&expectedResult, 0, 0, 20);
   COMPARE_MTX(expectedResult, interpolator(start, end, 1));

   D3DXMatrixTranslation(&expectedResult, 0, 0, 15);
   COMPARE_MTX(expectedResult, interpolator(start, end, 0.5f));
}

///////////////////////////////////////////////////////////////////////////////

TEST(MatrixInterpolator, rotation)
{
   D3DXMATRIX start;
   D3DXMATRIX end;

   D3DXMatrixRotationYawPitchRoll(&start, 0, 0, 0);
   D3DXMatrixRotationYawPitchRoll(&end, D3DXToRadian(90), 0, 0);

   MatrixInterpolator interpolator;

   COMPARE_MTX(start, interpolator(start, end, 0));
   COMPARE_MTX(end, interpolator(start, end, 1));

   D3DXMATRIX expectedResult;
   D3DXMatrixRotationYawPitchRoll(&expectedResult, D3DXToRadian(45), 0, 0);
   COMPARE_MTX(expectedResult, interpolator(start, end, 0.5f));
}

///////////////////////////////////////////////////////////////////////////////

TEST(WaypointCameraController, complexMovementx)
{
   D3DXMATRIX start;
   D3DXMATRIX end;

   D3DXMatrixRotationYawPitchRoll(&start, 0, 10, -30);
   end = D3DXMATRIX(0.5f,      0,        -0.866025f,  0,
                    0,         1,         0,          0,
                    0.866025f, 0,         0.5f,       0,
                    -5.980762f, 14.99999f, 24.999998f, 1);

   MatrixInterpolator interpolator;
   COMPARE_MTX(start, interpolator(start, end, 0));

   D3DXMATRIX result = interpolator(start, end, 1);
   COMPARE_MTX(end, result);
}

///////////////////////////////////////////////////////////////////////////////
