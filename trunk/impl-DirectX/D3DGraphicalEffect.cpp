#include "impl-DirectX\D3DGraphicalEffect.h"
#include "impl-DirectX\D3DRenderer.h"
#include "core-Renderer\Texture.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEffect::D3DGraphicalEffect(const std::string& name, 
                                       RenderingTargetsPolicy& policy,
                                       bool isTransparent,
                                       EffectDataSource* dataSource,
                                       IDirect3DDevice9& d3Device, 
                                       D3DRenderer& renderer,
                                       ID3DXEffect* effect)
      : GraphicalEffect(name, policy, isTransparent, dataSource),
      m_d3Device(d3Device),
      m_renderer(renderer),
      m_effect(effect)
{
   if (effect == NULL)
   {
      throw std::invalid_argument("NULL pointer instead an ID3DXEffect instance");
   }
   dynamic_cast<Subject<D3DRenderer, D3DGraphResourceOp>&>(m_renderer).attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEffect::~D3DGraphicalEffect()
{
   dynamic_cast<Subject<D3DRenderer, D3DGraphResourceOp>&>(m_renderer).detachObserver(*this);

   m_effect->Release();
   m_effect = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::update(D3DRenderer& renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::update(D3DRenderer& renderer, const D3DGraphResourceOp& operation)
{
   switch(operation)
   {
   case GRO_RELEASE_RES:
      {
         m_effect->OnLostDevice();
         break;
      }
   
   case GRO_CREATE_RES:
      {
         m_effect->OnResetDevice();
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::setTechnique(const std::string& technique)
{
   m_effect->SetTechnique(technique.c_str());
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, bool val)
{
   m_effect->SetBool(paramName.c_str(), val);
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, int val)
{
   m_effect->SetInt(paramName.c_str(), val);
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const Array<int>& val)
{
   m_effect->SetIntArray(paramName.c_str(), val, val.size());
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, float val)
{
   m_effect->SetFloat(paramName.c_str(), val);
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const Array<float>& val)
{
   m_effect->SetFloatArray(paramName.c_str(), val, val.size());
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const D3DXMATRIX& val)
{
   m_effect->SetMatrix(paramName.c_str(), &val);
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const Array<D3DXMATRIX>& val)
{
   m_effect->SetMatrixArray(paramName.c_str(), val, val.size());
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const std::string& val)
{
   m_effect->SetString(paramName.c_str(), val.c_str());
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, Texture& val)
{
   TextureImpl& textureImpl = val.getImpl();

   TTextureImpl<IDirect3DTexture9>* typedImpl = dynamic_cast<TTextureImpl<IDirect3DTexture9>*> (&textureImpl);
   if (typedImpl == NULL)
   {
      m_effect->SetTexture(paramName.c_str(), NULL);
   }
   else
   {
      m_effect->SetTexture(paramName.c_str(), &(typedImpl->get()));
   }
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const D3DXVECTOR4& val)
{
   m_effect->SetVector(paramName.c_str(), &val);
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::set(const std::string& paramName, const Array<D3DXVECTOR4>& val)
{
   m_effect->SetVectorArray(paramName.c_str(), val, val.size());
}

///////////////////////////////////////////////////////////////////////////////

unsigned int D3DGraphicalEffect::onBeginRendering()
{
   unsigned int passesCount;
   m_effect->Begin(&passesCount, 0);

   return passesCount;
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::endRendering()
{
   m_effect->End();
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::beginPass(const unsigned int& passIdx)
{
   m_effect->BeginPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void D3DGraphicalEffect::endPass(const unsigned int& passIdx)
{
   m_effect->EndPass();
}

///////////////////////////////////////////////////////////////////////////////
