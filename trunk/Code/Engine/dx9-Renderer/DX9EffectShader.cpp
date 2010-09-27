#include "dx9-Renderer\DX9EffectShader.h"
#include "dx9-Renderer\DX9Renderer.h"
#include "core-Renderer\ShaderTexture.h"
#include "core\File.h"
#include "dx9-Renderer\DXErrorParser.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

DX9EffectShader::DX9EffectShader(EffectShader& shader)
: m_shader(shader)
, m_renderer(NULL)
, m_d3Device(NULL)
, m_effect(NULL)
{
}

///////////////////////////////////////////////////////////////////////////////

DX9EffectShader::~DX9EffectShader()
{
   m_d3Device = NULL;

   if (m_renderer != NULL)
   {
      dynamic_cast<Subject<DX9Renderer, DX9GraphResourceOp>*>(m_renderer)->detachObserver(*this);
      m_renderer = NULL;
   }

   if (m_effect != NULL)
   {
      m_effect->Release();
      m_effect = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::initialize(Renderer& renderer)
{
   m_renderer = dynamic_cast<DX9Renderer*> (&renderer);
   if (m_renderer == NULL)
   {
      throw std::runtime_error("This implementation can work only with DX9Renderer");
   }

   if (m_effect != NULL)
   {
      m_effect->Release();
      m_effect = NULL;
   }

   // load the effect
   ID3DXBuffer* errorsBuf = NULL;

   const std::string& effectContents = m_shader.getScript();

   m_d3Device = &(m_renderer->getD3Device());
   HRESULT res = D3DXCreateEffect(m_d3Device, 
      effectContents.c_str(), 
      effectContents.length(),
      NULL, 
      NULL, 
      0, 
      NULL, 
      &m_effect, 
      &errorsBuf);

   if (FAILED(res) || (m_effect == NULL))
   {
      if (errorsBuf != NULL)
      {
         std::string compilationErrors = (const char*)errorsBuf->GetBufferPointer();
         errorsBuf->Release();
         throw std::runtime_error(std::string("Effect compilation error: ") + compilationErrors);
      }
      else
      {
         std::string errMsg = translateDxError( "Error while loading an effect", res );
         throw std::runtime_error(errMsg);
      }
   }

   // attach the instance as the renderer observer
   dynamic_cast<Subject<DX9Renderer, DX9GraphResourceOp>*>(m_renderer)->attachObserver(*this);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::update(DX9Renderer& renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::update(DX9Renderer& renderer, const DX9GraphResourceOp& operation)
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

void DX9EffectShader::setTechnique(const std::string& technique)
{
   m_effect->SetTechnique(technique.c_str());
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setBool(const std::string& paramName, bool val)
{
   m_effect->SetBool(paramName.c_str(), val);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setInt(const std::string& paramName, int val)
{
   m_effect->SetInt(paramName.c_str(), val);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setInt(const std::string& paramName, 
                             const int* arr, 
                             unsigned int size)
{
   m_effect->SetIntArray(paramName.c_str(), arr,size);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setFloat(const std::string& paramName, float val)
{
   m_effect->SetFloat(paramName.c_str(), val);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setFloat(const std::string& paramName, 
                                  const float* arr, 
                                  unsigned int size)
{
   m_effect->SetFloatArray(paramName.c_str(), arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setMtx(const std::string& paramName, 
                                const D3DXMATRIX& val)
{
   m_effect->SetMatrix(paramName.c_str(), &val);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setMtx(const std::string& paramName, 
                                const D3DXMATRIX* arr, 
                                unsigned int size)
{
   m_effect->SetMatrixArray(paramName.c_str(), arr, size);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setString(const std::string& paramName, 
                                   const std::string& val)
{
   m_effect->SetString(paramName.c_str(), val.c_str());
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setTexture(const std::string& paramName, ShaderTexture& val)
{
   IDirect3DTexture9* texture = reinterpret_cast<IDirect3DTexture9*> (val.getPlatformSpecific());
   m_effect->SetTexture(paramName.c_str(), texture);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setVec4(const std::string& paramName, 
                                 const D3DXVECTOR4& val)
{
   m_effect->SetVector(paramName.c_str(), &val);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::setVec4(const std::string& paramName, 
                                 const D3DXVECTOR4* arr, 
                                 unsigned int size)
{
   m_effect->SetVectorArray(paramName.c_str(), arr, size);
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DX9EffectShader::beginRendering()
{
   unsigned int passesCount;
   m_effect->Begin(&passesCount, 0);

   return passesCount;
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::endRendering()
{
   m_effect->End();
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::beginPass(unsigned int passIdx)
{
   m_effect->BeginPass(passIdx);
}

///////////////////////////////////////////////////////////////////////////////

void DX9EffectShader::endPass(unsigned int passIdx)
{
   m_effect->EndPass();
}

///////////////////////////////////////////////////////////////////////////////
