#include "core-Renderer\ProjCalc3D.h"



///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX ProjCalc3D::calculate(float fov, float aspectRatio,
                                 float nearZPlane, float farZPlane)
{
   D3DXMATRIX mtx;

   D3DXMatrixPerspectiveFovLH(&mtx, fov, aspectRatio, 
                           nearZPlane, farZPlane);

   return mtx;
}

///////////////////////////////////////////////////////////////////////////////
