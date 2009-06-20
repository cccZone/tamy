#pragma once

#include "core-Renderer\ParticleSystem.h"
#include <d3d9.h>
#include <d3dx9.h>


///////////////////////////////////////////////////////////////////////////////

struct ParticleVertex
{
   D3DXVECTOR3 coords;
   float size;
   DWORD color;

   ParticleVertex(D3DXVECTOR3 _coords, float _size, DWORD _color) 
      : coords(_coords), size(_size), color(_color)
   {}
};

///////////////////////////////////////////////////////////////////////////////

class D3DParticleSystem : public ParticleSystem
{
private:
   IDirect3DDevice9& m_d3Device;
   IDirect3DVertexBuffer9* m_vertexBuffer;

   DWORD m_particlesCount;
   D3DXMATRIX m_mtxIdentity;
   float m_defaultPointSize;
   float m_linearScalingFactor;

public:
   D3DParticleSystem(IDirect3DDevice9& d3Device, 
                     const std::string& name,
                     bool isDynamic, 
                     Material& material,
                     unsigned int particlesCount);
   ~D3DParticleSystem();

   void render();

protected:
   void onUpdate();
   void onActivateParticle(unsigned int idx);
};

///////////////////////////////////////////////////////////////////////////////
