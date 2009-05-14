#pragma once

#include "core-Renderer\MaterialOperationImplementation.h"
#include <d3d9.h>
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

/**
 * This is a DirectX imlpementation of a material stagerelated functionality
 */
class D3DAlphaOperationImplementation : public MaterialOperationImplementation
{
private:
   IDirect3DDevice9& m_d3Device;

public:
   D3DAlphaOperationImplementation(IDirect3DDevice9& d3Device);

   void setLightReflectingProperties(unsigned char stage, unsigned char argIdx);

   void setTexture(unsigned char stage, unsigned char argIdx);

   void setPreviousStageResults(unsigned char stage, unsigned char argIdx);

   void disableArgument(unsigned char stage, unsigned char argIdx);

   void disableStage(unsigned char stage);

   void selectArg1(unsigned char stage);

   void selectArg2(unsigned char stage);

   void multiply(unsigned char stage);

   void multiply2X(unsigned char stage);

   void multiply4X(unsigned char stage);

   void add(unsigned char stage);

   void addSigned(unsigned char stage);

   void addSigned2X(unsigned char stage);

   void addSmooth(unsigned char stage);

   void subtract(unsigned char stage);

   void multiplyAdd(unsigned char stage);

private:
   D3DTEXTURESTAGESTATETYPE translateArgNum(unsigned char argIdx) const
   {
      D3DTEXTURESTAGESTATETYPE arg;
      switch (argIdx)
      {
      case 0: arg = D3DTSS_ALPHAARG1; break;
      case 1: arg = D3DTSS_ALPHAARG2; break;
      default: throw std::runtime_error("Invalid argument index"); break;
      }

      return arg;
   }
};

///////////////////////////////////////////////////////////////////////////////
