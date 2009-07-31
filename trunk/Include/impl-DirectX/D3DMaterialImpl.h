#pragma once

#include <d3d9.h>
#include "core-Renderer\MaterialImpl.h"


///////////////////////////////////////////////////////////////////////////////

enum SourceCode;

///////////////////////////////////////////////////////////////////////////////

class D3DMaterialImpl : public MaterialImpl
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DMATERIAL9 m_material;

public:
   D3DMaterialImpl(IDirect3DDevice9& d3Device);

   void setTransparency(bool enable);
   
   void setLRP(LightReflectingProperties& lrp);

   void setTexture(unsigned char stageIndex, TextureImpl& textureImpl);

   void setColorOperation(unsigned char stageIndex, MaterialOperation& colorOp);

   void setAlphaOperation(unsigned char stageIndex, MaterialOperation& alphaOp);

   void setCoordsOperation(unsigned char stageIndex, CoordinatesOperation& coordsOp);

private:
   DWORD translateArg(SourceCode code) const;

};

///////////////////////////////////////////////////////////////////////////////
