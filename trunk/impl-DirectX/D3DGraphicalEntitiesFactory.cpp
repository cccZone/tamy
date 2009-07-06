#include "impl-DirectX\D3DGraphicalEntitiesFactory.h"
#include "impl-DirectX\D3DGraphicalEntity.h"
#include "impl-DirectX\D3DSkinnedGraphicalEntity.h"
#include "impl-DirectX\D3DRenderer.h"
#include "core-Renderer\LitVertex.h"
#include "impl-DirectX\D3DLightReflectingProperties.h"
#include "impl-DirectX\D3DLight.h"
#include "impl-DirectX\D3DSkyBox.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "impl-DirectX\D3DColorOperationImplementation.h"
#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include "impl-DirectX\D3DMaterial.h"
#include "impl-DirectX\D3DTexture.h"
#include "impl-DirectX\D3DEmptyTexture.h"
#include "impl-DirectX\D3DParticleSystem.h"


///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEntitiesFactory::D3DGraphicalEntitiesFactory(const std::string& texturesPath,
                                                         IDirect3DDevice9& d3Device,
                                                         D3DRenderer& renderer)
      : GraphicalEntitiesFactory(texturesPath),
      m_d3Device(d3Device),
      m_renderer(renderer),
      m_colorOpImpl(new D3DColorOperationImplementation(d3Device)),
      m_alphaOpImpl(new D3DAlphaOperationImplementation(d3Device))
{
}

///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEntitiesFactory::~D3DGraphicalEntitiesFactory()
{
   delete m_colorOpImpl;
   m_colorOpImpl = NULL;

   delete m_alphaOpImpl;
   m_alphaOpImpl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

Light* D3DGraphicalEntitiesFactory::createLight(const std::string& name)
{
   return new D3DLight(name, m_d3Device);
}

///////////////////////////////////////////////////////////////////////////////

LightReflectingProperties* D3DGraphicalEntitiesFactory::createLightReflectingProperties()
{
   return new D3DLightReflectingProperties(m_d3Device);
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity* D3DGraphicalEntitiesFactory::createGraphicalEntity(
                                                           const std::string& name,
                                                           const std::vector<LitVertex>& vertices,
                                                           const std::list<Face<USHORT> >& faces,
                                                           const std::vector<Material*>& registeredMaterials)
{
    return new D3DGraphicalEntity<LitVertex>(name, m_d3Device, vertices, faces, registeredMaterials);
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity* D3DGraphicalEntitiesFactory::createSkinnedGraphicalEntity(
                                                           const std::string& name,
                                                           const std::vector<LitVertex>& vertices,
                                                           const std::list<Face<USHORT> >& faces,
                                                           const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                           const std::vector<SkinBoneDefinition>& skinBones,
                                                           const std::vector<Material*>& registeredMaterials)
{
   return new D3DSkinnedGraphicalEntity<LitVertex>(name, 
                                                   m_d3Device, 
                                                   vertices, 
                                                   faces, 
                                                   bonesInfluencingAttribute, 
                                                   skinBones, 
                                                   registeredMaterials);
}

///////////////////////////////////////////////////////////////////////////////

SkyBox* D3DGraphicalEntitiesFactory::createSkyBox()
{
   IDirect3DVertexBuffer9* vb = m_renderer.createVertexBuffer(sizeof(D3DSkyBoxVertex) * 24, 
                                                              D3DUSAGE_WRITEONLY,
                                                              D3DSkyBoxVertex::FVF,
                                                              D3DPOOL_MANAGED);

   D3DSkyBoxVertex* pVertex = NULL;
   if (FAILED(vb->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD)))
   {
      throw std::logic_error(
         std::string("Cannot lock a vertex buffer"));
   }

   // Front quad (remember all quads point inward)
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 0.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 1.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 0.0f, 1.0f );
   *pVertex++  = D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 1.0f, 1.0f );

   // Back Quad
   *pVertex++  = D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 0.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 1.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 0.0f, 1.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 1.0f, 1.0f );

   // Left Quad
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 0.0f, 0.0f );
   *pVertex++  = D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 1.0f, 1.0f );

   // Right Quad
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 0.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 1.0f, 1.0f );

   // Top Quad
   *pVertex++ = D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 0.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 1.0f, 1.0f );

   // Bottom Quad
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 0.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 1.0f, 0.0f );
   *pVertex++ = D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 0.0f, 1.0f );
   *pVertex++ = D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 1.0f, 1.0f );

   vb->Unlock();

   return new D3DSkyBox(m_d3Device, vb);

}

///////////////////////////////////////////////////////////////////////////////

Texture* D3DGraphicalEntitiesFactory::loadTexture(const std::string& path, const std::string& fileName)
{
   IDirect3DTexture9* loadedTex = NULL;
   std::string fullPathToTexture = path + std::string("\\") + fileName;

   HRESULT res = D3DXCreateTextureFromFileEx(&m_d3Device,
                                             fullPathToTexture.c_str(),
                                             D3DX_DEFAULT, D3DX_DEFAULT,
                                             D3DX_DEFAULT, 0, D3DFMT_UNKNOWN,
                                             D3DPOOL_MANAGED, 
                                             D3DX_DEFAULT, D3DX_DEFAULT,
                                             D3DCOLOR(), NULL, NULL, &loadedTex);
   if (FAILED(res))
   {
      throw std::logic_error(std::string("Can't load texture from file ") + fullPathToTexture);
   }

   Texture* tex = new D3DTexture(fileName, m_d3Device, *loadedTex);
   loadedTex->Release();

   return tex;
}

//////////////////////////////////////////////////////////////////////////////

Texture* D3DGraphicalEntitiesFactory::createEmptyTexture()
{
   return new D3DEmptyTexture(m_d3Device);
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage* D3DGraphicalEntitiesFactory::createMaterialStage(Texture& tex,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2)
{
   return new MaterialStage(tex,
            new MaterialOperation(*m_colorOpImpl, 
                                  colorOp, colorArg1, colorArg2),
            new MaterialOperation(*m_alphaOpImpl, 
                                  alphaOp, alphaArg1, alphaArg2));
}

///////////////////////////////////////////////////////////////////////////////

Material* D3DGraphicalEntitiesFactory::createMaterial(
                                             const std::string& name,
                                             unsigned int index, 
                                             LightReflectingProperties* lrp)
{
   return new D3DMaterial(m_d3Device, 
                          name,
                          lrp, 
                          *m_alphaOpImpl,
                          *m_colorOpImpl,
                          index);
}

///////////////////////////////////////////////////////////////////////////////

ParticleSystem* D3DGraphicalEntitiesFactory::createParticleSystem(
                                              const std::string& name, 
                                              bool isDynamic, 
                                              Material& material,
                                              unsigned int particlesCount)
{
   return new D3DParticleSystem(m_d3Device,
                                name, isDynamic, material, particlesCount);
}

///////////////////////////////////////////////////////////////////////////////
