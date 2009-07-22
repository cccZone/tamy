#include "impl-DirectX\D3DParticleSystem.h"
#include "core\Assert.h"
#include "core-Renderer\Particle.h"


///////////////////////////////////////////////////////////////////////////////

#define D3DFVF_PARTICLEVERTEX (D3DFVF_XYZ | D3DFVF_PSIZE | D3DFVF_DIFFUSE)

///////////////////////////////////////////////////////////////////////////////

D3DParticleSystem::D3DParticleSystem(IDirect3DDevice9& d3Device, 
                                     const std::string& name,
                                     bool isDynamic, 
                                     RenderingTechnique& technique,
                                     unsigned int particlesCount)
      : ParticleSystem(name, isDynamic, technique, particlesCount),
      m_d3Device(d3Device),
      m_vertexBuffer(NULL),
      m_particlesCount(0),
      m_defaultPointSize(1.f),
      m_linearScalingFactor(1.f)
{
   // create a new vertex buffer
   HRESULT res = m_d3Device.CreateVertexBuffer(particlesCount * sizeof(ParticleVertex), 
                                               D3DUSAGE_WRITEONLY,
                                               D3DFVF_PARTICLEVERTEX, 
                                               D3DPOOL_MANAGED, 
                                               &m_vertexBuffer, 
                                               NULL);
   ASSERT(SUCCEEDED(res), "Cannot create a vertex buffer for a particle system");

   // copy the data about the particles there
   ParticleVertex* particlesVB = NULL;
   res = m_vertexBuffer->Lock(0, particlesCount * sizeof(ParticleVertex), 
                              (void**)&particlesVB, 0);
   ASSERT(SUCCEEDED(res), "Cannot lock the particles vertex buffer");
   ASSERT(particlesVB != NULL, "Particles buffer wasn't locked");

   for (unsigned int i = 0; i < particlesCount; ++i, ++particlesVB)
   {
      *particlesVB = ParticleVertex(D3DXVECTOR3(0, 0, 0), 0, 0);
   }
   res = m_vertexBuffer->Unlock();
   ASSERT(SUCCEEDED(res), "Cannot unlock the particles vertex buffer");


   // initialize the matrix that will be used as the global matrix 
   // for drawing point sprites
   D3DXMatrixIdentity(&m_mtxIdentity);
}

///////////////////////////////////////////////////////////////////////////////

D3DParticleSystem::~D3DParticleSystem()
{
   if (m_vertexBuffer != NULL)
   {
      m_vertexBuffer->Release();
      m_vertexBuffer = NULL;
   }

   m_particlesCount = 0;
}

///////////////////////////////////////////////////////////////////////////////

void D3DParticleSystem::render()
{
   unsigned int count = getActiveParticlesCount();
   if (count == 0) {return;}

   updateParticles();

   // initialize the renderer
   m_d3Device.SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&m_defaultPointSize));
   m_d3Device.SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&m_linearScalingFactor));
   m_d3Device.SetRenderState(D3DRS_POINTSPRITEENABLE, true);
   m_d3Device.SetRenderState(D3DRS_POINTSCALEENABLE, true);
   m_d3Device.SetRenderState(D3DRS_LIGHTING, false);
  
   // render the particles
   m_d3Device.SetFVF(D3DFVF_PARTICLEVERTEX);
   m_d3Device.SetStreamSource(0, m_vertexBuffer, 0, sizeof(ParticleVertex));
   m_d3Device.SetTransform(D3DTS_WORLD, &m_mtxIdentity);
   m_d3Device.DrawPrimitive(D3DPT_POINTLIST, 0, count);

   // deinitialize the renderer
   m_d3Device.SetRenderState(D3DRS_LIGHTING, true);
   m_d3Device.SetRenderState(D3DRS_POINTSPRITEENABLE, false);
   m_d3Device.SetRenderState(D3DRS_POINTSCALEENABLE, false);
}

///////////////////////////////////////////////////////////////////////////////

void D3DParticleSystem::updateParticles()
{  
   // update the positions of the particles
   ParticleVertex* particlesVB = NULL;
   HRESULT res = m_vertexBuffer->Lock(0, m_particlesCount * sizeof(ParticleVertex), 
                                      (void**)&particlesVB, 0);
   ASSERT(SUCCEEDED(res), "Cannot lock the particles vertex buffer");
   ASSERT(particlesVB != NULL, "Particles buffer wasn't locked");

   const Particle* particle = NULL;
   unsigned int count = getActiveParticlesCount();
   for (unsigned int i = 0; i < count; ++i, ++particlesVB)
   {
      particle = &(getParticle(i));
      const Color& color = particle->color;
      *particlesVB = ParticleVertex(particle->position, 
                                    particle->size, 
                                    D3DCOLOR_COLORVALUE(color.r, color.g, color.b, color.a));
   }
   res = m_vertexBuffer->Unlock();
   ASSERT(SUCCEEDED(res), "Cannot unlock the particles vertex buffer");
}

///////////////////////////////////////////////////////////////////////////////
