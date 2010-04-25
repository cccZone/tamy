#pragma once

#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class ProjectionCalculator
{
public:
   virtual ~ProjectionCalculator() {}

   virtual D3DXMATRIX calculate(float fov, float aspectRatio,
                                float nearZPlane, float farZPlane) = 0;
};

///////////////////////////////////////////////////////////////////////////////
