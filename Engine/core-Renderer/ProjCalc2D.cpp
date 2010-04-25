#include "core-Renderer\ProjCalc2D.h"
#include <math.h>


///////////////////////////////////////////////////////////////////////////////

D3DXMATRIX ProjCalc2D::calculate(float fov, float aspectRatio,
                                 float nearZPlane, float farZPlane)
{
   D3DXMATRIX mtx;

   // why is it 2.f that I need to set up for the width and height - I don't know.
   // But it works - with these params the coordinates that fit into the vieport
   // fall directly into the <-1, 1> interval for both axis
   D3DXMatrixOrthoLH(&mtx, 2.f, 2.f, nearZPlane, farZPlane);

   return mtx;
}

///////////////////////////////////////////////////////////////////////////////
