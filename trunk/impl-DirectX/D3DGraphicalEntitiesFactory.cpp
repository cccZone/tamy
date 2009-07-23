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
#include "core-Renderer\Material.h"
#include "core-Renderer\ManagedTexture.h"
#include "impl-DirectX\D3DColorOperationImplementation.h"
#include "impl-DirectX\D3DAlphaOperationImplementation.h"
#include "impl-DirectX\D3DTexture.h"
#include "impl-DirectX\D3DParticleSystem.h"
#include "impl-DirectX\D3DTransparencyEnabler.h"
#include "impl-DirectX\D3DCoordinatesOperation.h"
#include "impl-DirectX\D3DDefaultRenderingTarget.h"
#include "impl-DirectX\D3DTextureRenderingTarget.h"
#include "impl-DirectX\D3DStageTextureRenderer.h"
#include "impl-DirectX\D3DGraphicalEffect.h"
#include "impl-DirectX\D3DPostProcessEffectNode.h"


///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEntitiesFactory::D3DGraphicalEntitiesFactory(const std::string& texturesPath,
                                                         IDirect3DDevice9& d3Device,
                                                         D3DRenderer& renderer)
      : GraphicalEntitiesFactory(texturesPath, renderer),
      m_d3Device(d3Device),
      m_renderer(renderer),
      m_colorOpImpl(new D3DColorOperationImplementation(d3Device)),
      m_alphaOpImpl(new D3DAlphaOperationImplementation(d3Device)),
      m_transparencyEnabler(new D3DTransparencyEnabler(d3Device)),
      m_stageTextureRenderer(new D3DStageTextureRenderer(d3Device))
{
}

///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEntitiesFactory::~D3DGraphicalEntitiesFactory()
{
   delete m_stageTextureRenderer;
   m_stageTextureRenderer = NULL;

   delete m_transparencyEnabler;
   m_transparencyEnabler = NULL;

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
                                                           const std::vector<RenderingTechnique*>& techniques)
{
    return new D3DGraphicalEntity<LitVertex>(name, m_d3Device, vertices, faces, techniques);
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity* D3DGraphicalEntitiesFactory::createSkinnedGraphicalEntity(
                                                           const std::string& name,
                                                           const std::vector<LitVertex>& vertices,
                                                           const std::list<Face<USHORT> >& faces,
                                                           const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                           const std::vector<SkinBoneDefinition>& skinBones,
                                                           const std::vector<RenderingTechnique*>& techniques)
{
   return new D3DSkinnedGraphicalEntity<LitVertex>(name, 
                                                   m_d3Device, 
                                                   vertices, 
                                                   faces, 
                                                   bonesInfluencingAttribute, 
                                                   skinBones, 
                                                   techniques);
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

   return new D3DSkyBox(*m_stageTextureRenderer, m_renderer.getRenderingTargetsPolicy(), m_d3Device, vb);

}

///////////////////////////////////////////////////////////////////////////////

Texture* D3DGraphicalEntitiesFactory::loadTexture(const std::string& path, const std::string& fileName)
{
   IDirect3DTexture9* loadedTex = NULL;
   std::string fullPathToTexture = path + std::string("\\") + fileName;

   D3DFORMAT texFormat = m_renderer.getOptimalTextureFormat();
   HRESULT res;

   res = D3DXCreateTextureFromFileEx(&m_d3Device,
                                     fullPathToTexture.c_str(),
                                     D3DX_DEFAULT, D3DX_DEFAULT,
                                     D3DX_DEFAULT, 0, texFormat,
                                     D3DPOOL_MANAGED, 
                                     D3DX_DEFAULT, D3DX_DEFAULT,
                                     D3DCOLOR(), NULL, NULL, &loadedTex);

   if (FAILED(res))
   {
      std::string errorMsg = std::string("Can't load texture from file ") + fullPathToTexture;
      switch(res)
      {
      case D3DERR_INVALIDCALL:
         {
            errorMsg += " due to invalid parameters passed";
            break;
         }

      case D3DERR_NOTAVAILABLE:
         {
            errorMsg += " - this format is unavailable";
            break;
         }

      case D3DERR_OUTOFVIDEOMEMORY:
         {
            errorMsg += " due to the lack of video memory";
            break;
         }

      case D3DXERR_INVALIDDATA:
         {
            errorMsg += " due to invalid data";
            break;
         }

      case E_OUTOFMEMORY:
         {
            errorMsg += " due to the lack of system memory";
            break;
         }

      default:
         {
            errorMsg += " for unknown reason";
            break;
         }
      }

      throw std::logic_error(errorMsg);
   }

   TTextureImpl<IDirect3DTexture9>* texImpl = new TTextureImpl<IDirect3DTexture9>();
   texImpl->set(loadedTex);
   Texture* tex = new ManagedTexture(fileName, texImpl);

   return tex;
}

//////////////////////////////////////////////////////////////////////////////

Texture* D3DGraphicalEntitiesFactory::createEmptyTexture()
{
   return new ManagedTexture("", new TTextureImpl<IDirect3DTexture9>());
}

///////////////////////////////////////////////////////////////////////////////

MaterialStage* D3DGraphicalEntitiesFactory::createMaterialStage(Texture& tex,
                                      MatOpCode colorOp, SourceCode colorArg1, SourceCode colorArg2,
                                      MatOpCode alphaOp, SourceCode alphaArg1, SourceCode alphaArg2,
                                      CoordsOpCode coordsOp)
{
   return new MaterialStage(tex,
            new MaterialOperation(*m_colorOpImpl, 
                                  colorOp, colorArg1, colorArg2),
            new MaterialOperation(*m_alphaOpImpl, 
                                  alphaOp, alphaArg1, alphaArg2),
            new D3DCoordinatesOperation(m_d3Device, coordsOp),
            *m_stageTextureRenderer);
}

///////////////////////////////////////////////////////////////////////////////

Material* D3DGraphicalEntitiesFactory::createMaterialImpl(
                                             const std::string& name,
                                             RenderingTargetsPolicy& policy,
                                             LightReflectingProperties* lrp)
{
   return new Material(name, policy, lrp, *m_alphaOpImpl, *m_colorOpImpl, *m_transparencyEnabler);
}

///////////////////////////////////////////////////////////////////////////////

ParticleSystem* D3DGraphicalEntitiesFactory::createParticleSystem(
                                              const std::string& name, 
                                              bool isDynamic, 
                                              RenderingTechnique& renderingTechnique,
                                              unsigned int particlesCount)
{
   return new D3DParticleSystem(m_d3Device,
                                name, isDynamic, renderingTechnique, particlesCount);
}

///////////////////////////////////////////////////////////////////////////////

RenderingTarget* D3DGraphicalEntitiesFactory::createDefaultRenderingTarget()
{
   return new D3DDefaultRenderingTarget(m_d3Device, m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

TextureRenderingTarget* 
D3DGraphicalEntitiesFactory::createTextureRenderingTarget(const std::string& name)
{
   return new D3DTextureRenderingTarget(name, m_d3Device, m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

PostProcessEffectNode* 
D3DGraphicalEntitiesFactory::createPostProcessEffectNode(const std::string& name,
                                                         RenderingTechnique& technique)
{
   return new D3DPostProcessEffectNode(name, technique, m_d3Device, m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEffect* D3DGraphicalEntitiesFactory::createEffectImpl(const std::string& name, 
                                                               RenderingTargetsPolicy& policy,
                                                               bool isTransparent,
                                                               EffectDataSource* dataSource)
{
   ID3DXEffect* effect = NULL;
	ID3DXBuffer* errorsBuf = NULL;
	HRESULT res = D3DXCreateEffectFromFile(&m_d3Device, 
		                                    name.c_str(),
		                                    NULL, 
		                                    NULL, 
		                                    0, 
		                                    NULL, 
		                                    &effect, 
		                                    &errorsBuf);

   if (FAILED(res) || (effect == NULL))
	{
      std::string compilationErrors = (const char*)errorsBuf->GetBufferPointer();
      throw std::runtime_error(std::string("Effect compilation error: ") + compilationErrors);
	}

   return new D3DGraphicalEffect(name, 
                                 policy, 
                                 isTransparent, 
                                 dataSource, 
                                 m_d3Device, 
                                 m_renderer, 
                                 effect);
}

///////////////////////////////////////////////////////////////////////////////
