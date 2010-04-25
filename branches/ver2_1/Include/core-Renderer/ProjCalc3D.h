#pragma once

#include "core-Renderer\ProjectionCalculator.h"


///////////////////////////////////////////////////////////////////////////////

class ProjCalc3D : public ProjectionCalculator
{
public:
   D3DXMATRIX calculate(float fov, float aspectRatio,
                        float nearZPlane, float farZPlane);
};

///////////////////////////////////////////////////////////////////////////////
