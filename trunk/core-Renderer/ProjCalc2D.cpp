#include "core-Renderer\ProjCalc2D.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX ProjCalc2D::calculate(float fov, float aspectRatio,
                                 float nearZPlane, float farZPlane)
{
   D3DXMATRIX mtx;

   float nearPlaneHeight = (2.f * nearZPlane) * tan(fov / 2.f);
   float nearPlaneWidth = nearPlaneHeight * aspectRatio;
   D3DXMatrixOrthoLH(&mtx, nearPlaneWidth, nearPlaneHeight,
                     nearZPlane, farZPlane);

   return mtx;
}

///////////////////////////////////////////////////////////////////////////////
