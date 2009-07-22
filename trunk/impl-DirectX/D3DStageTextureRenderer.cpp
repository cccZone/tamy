#include "impl-DirectX\D3DStageTextureRenderer.h"
#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

D3DStageTextureRenderer::D3DStageTextureRenderer(IDirect3DDevice9& d3Device)
      : m_d3Device(d3Device)
{}

///////////////////////////////////////////////////////////////////////////////

void D3DStageTextureRenderer::setForRendering(unsigned int stageIdx, 
                                              TextureImpl& textureImpl)
{
   TTextureImpl<IDirect3DTexture9>* typedImpl = dynamic_cast<TTextureImpl<IDirect3DTexture9>*> (&textureImpl);
   if (typedImpl == NULL)
   {
      m_d3Device.SetTexture(stageIdx, NULL);
   }
   else
   {
      m_d3Device.SetTexture(stageIdx, &(typedImpl->get()));
   }
}

///////////////////////////////////////////////////////////////////////////////
