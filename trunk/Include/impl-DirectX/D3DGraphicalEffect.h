#pragma once

#include "core-Renderer\GraphicalEffect.h"
#include "core\Observer.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

class D3DRenderer;
enum D3DGraphResourceOp;

///////////////////////////////////////////////////////////////////////////////

class D3DGraphicalEffect : public GraphicalEffect,
                           public Observer<D3DRenderer, D3DGraphResourceOp>
{
private:
   IDirect3DDevice9& m_d3Device;
   D3DRenderer& m_renderer;
   ID3DXEffect* m_effect;

public:
   D3DGraphicalEffect(const std::string& name, 
                      IDirect3DDevice9& d3Device,
                      D3DRenderer& renderer,
                      ID3DXEffect* effect);
   ~D3DGraphicalEffect();

   void setTechnique(const std::string& material);
   void setBool(const std::string& paramName, bool val);
   void setInt(const std::string& paramName, int val);
   void setInt(const std::string& paramName, const int* arr, unsigned int size);
   void setFloat(const std::string& paramName, float val);
   void setFloat(const std::string& paramName, const float* arr, unsigned int size);
   void setMtx(const std::string& paramName, const D3DXMATRIX& val);
   void setMtx(const std::string& paramName, const D3DXMATRIX* arr, unsigned int size);
   void setString(const std::string& paramName, const std::string& val);
   void setTexture(const std::string& paramName, Texture& val);
   void setVec4(const std::string& paramName, const D3DXVECTOR4& val);
   void setVec4(const std::string& paramName, const D3DXVECTOR4* arr, unsigned int size);

   void update(D3DRenderer& renderer);
   void update(D3DRenderer& renderer, const D3DGraphResourceOp& operation);

protected:
   unsigned int beginRendering();
   void endRendering();
   void beginPass(const unsigned int& passIdx);
   void endPass(const unsigned int& passIdx);
};

///////////////////////////////////////////////////////////////////////////////
