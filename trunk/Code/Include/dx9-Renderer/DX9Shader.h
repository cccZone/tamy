#pragma once

#include "core-Renderer\Shader.h"
#include "core\Observer.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class DX9Renderer;
enum DX9GraphResourceOp;

///////////////////////////////////////////////////////////////////////////////

class DX9Shader : public ShaderImpl,
                  public Observer<DX9Renderer, DX9GraphResourceOp>
{
private:
   Shader& m_shader;
   DX9Renderer* m_renderer;
   IDirect3DDevice9* m_d3Device;
   ID3DXEffect* m_effect;

public:
   DX9Shader(Shader& shader);
   ~DX9Shader();

   // -------------------------------------------------------------------------
   // RendererObjectImpl implementation
   // -------------------------------------------------------------------------
   void initialize(Renderer& renderer);

   // -------------------------------------------------------------------------
   // ShaderImpl implementation
   // -------------------------------------------------------------------------
   void setTechnique(const std::string& technique);
   void setBool(const std::string& paramName, bool val);
   void setInt(const std::string& paramName, int val);
   void setInt(const std::string& paramName, const int* arr, unsigned int size);
   void setFloat(const std::string& paramName, float val);
   void setFloat(const std::string& paramName, const float* arr, unsigned int size);
   void setMtx(const std::string& paramName, const D3DXMATRIX& val);
   void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size);
   void setString(const std::string& paramName, const std::string& val);
   void setTexture(const std::string& paramName, ShaderTexture& val);
   void setVec4(const std::string& paramName, const D3DXVECTOR4& val);
   void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size);
   unsigned int beginRendering();
   void endRendering();
   void beginPass(unsigned int passIdx);
   void endPass(unsigned int passIdx);

   // -------------------------------------------------------------------------
   // Observer implementation
   // -------------------------------------------------------------------------
   void update(DX9Renderer& renderer);
   void update(DX9Renderer& renderer, const DX9GraphResourceOp& operation);
};

///////////////////////////////////////////////////////////////////////////////
