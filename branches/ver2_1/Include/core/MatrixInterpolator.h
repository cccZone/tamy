#pragma once

#include <d3dx9.h>
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

class MatrixInterpolator
{
public:
   D3DXMATRIX operator()(const D3DXMATRIX& start, 
                         const D3DXMATRIX& end, 
                         float advancement) const
   {
      ASSERT(advancement <= 1, "The value for advancement should be <= 1");
      ASSERT(advancement >= 0, "The value for advancement should be >= 0");

      // translation
      D3DXMATRIX translation;

      D3DXVECTOR3 startPos(start._41, start._42, start._43);
      D3DXVECTOR3 endPos(end._41, end._42, end._43);
      D3DXVECTOR3 resultPos = startPos + (endPos - startPos) * advancement;

      D3DXMatrixTranslation(&translation, resultPos.x, resultPos.y, resultPos.z);

      // rotation
      D3DXQUATERNION startQuat, endQuat;
      D3DXQuaternionRotationMatrix(&startQuat, &start);
      D3DXQuaternionRotationMatrix(&endQuat, &end);

      D3DXQUATERNION resultQuat;
      D3DXQuaternionSlerp(&resultQuat, &startQuat, &endQuat, advancement);

      D3DXMATRIX rotation;
      D3DXMatrixRotationQuaternion(&rotation, &resultQuat);

      // calculate the final result
      D3DXMATRIX result = rotation;
      D3DXMatrixMultiply(&result, &result, &translation);

      return result;

   }
};

///////////////////////////////////////////////////////////////////////////////
