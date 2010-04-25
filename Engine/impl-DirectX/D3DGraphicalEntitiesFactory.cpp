#include "impl-DirectX\D3DGraphicalEntitiesFactory.h"
#include "impl-DirectX\D3DGraphicalEntity.h"
#include "impl-DirectX\D3DSkinnedGraphicalEntity.h"
#include "impl-DirectX\D3DRenderer.h"
#include "core-Renderer\LitVertex.h"
#include "impl-DirectX\D3DSkyBoxSide.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\ManagedTexture.h"
#include "impl-DirectX\D3DTexture.h"
#include "impl-DirectX\D3DParticleSystem.h"
#include "impl-DirectX\D3DDefaultRenderingTarget.h"
#include "impl-DirectX\D3DTextureRenderingTarget.h"
#include "impl-DirectX\D3DGraphicalEffect.h"
#include "impl-DirectX\D3DPostProcessEffectRenderable.h"
#include "impl-DirectX\D3DRendererImpl.h"
#include "impl-DirectX\D3DRenderingTargetsCleaner.h"
#include "impl-DirectX\D3DStaticGeometryRenderable.h"
#include "impl-DirectX\D3DFont.h"
#include "core\Filesystem.h"
#include "core\StreamBuffer.h"


///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEntitiesFactory::D3DGraphicalEntitiesFactory(Filesystem& filesystem,
                                                         IDirect3DDevice9& d3Device,
                                                         D3DRenderer& renderer)
      : GraphicalEntitiesFactory(filesystem, renderer),
      m_d3Device(d3Device),
      m_renderer(renderer)
{
}

///////////////////////////////////////////////////////////////////////////////

D3DGraphicalEntitiesFactory::~D3DGraphicalEntitiesFactory()
{
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEntity* D3DGraphicalEntitiesFactory::createGraphicalEntity(
                                                           const std::string& name,
                                                           const std::vector<LitVertex>& vertices,
                                                           const std::vector<Face >& faces,
                                                           const std::vector<Material*>& materials)
{
    return new D3DGraphicalEntity<LitVertex>(name, m_d3Device, vertices, faces, materials);
}

///////////////////////////////////////////////////////////////////////////////

SkinnedGraphicalEntity* D3DGraphicalEntitiesFactory::createSkinnedGraphicalEntity(
                                                           const std::string& name,
                                                           const std::vector<LitVertex>& vertices,
                                                           const std::vector<Face >& faces,
                                                           const std::vector<BonesInfluenceDefinition>& bonesInfluencingAttribute,
                                                           const std::vector<SkinBoneDefinition>& skinBones,
                                                           const std::vector<Material*>& materials)
{
   return new D3DSkinnedGraphicalEntity<LitVertex>(name, 
                                                   m_d3Device, 
                                                   vertices, 
                                                   faces, 
                                                   bonesInfluencingAttribute, 
                                                   skinBones, 
                                                   materials);
}

///////////////////////////////////////////////////////////////////////////////

StaticGeometryRenderable* 
D3DGraphicalEntitiesFactory::createStaticGeometry(Material& material,
                                                  const std::vector<LitVertex>& vertices,
                                                  const std::vector<Face >& faces)
{
   return new D3DStaticGeometryRenderable(material,
                                          vertices, 
                                          faces, 
                                          m_d3Device);
}

///////////////////////////////////////////////////////////////////////////////

SkyBoxSide* D3DGraphicalEntitiesFactory::createSkyBoxSide(SkyBoxSideId side, Texture* tex)
{
   static D3DSkyBoxVertex vertices[] = 
   {
      // Front quad (remember all quads point inward)
      D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 0.0f, 0.0f ),
      D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 1.0f, 0.0f ),
      D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 0.0f, 1.0f ),
      D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 1.0f, 1.0f ),

      // Back Quad
      D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 0.0f, 0.0f ),
      D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 1.0f, 0.0f ),
      D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 0.0f, 1.0f ),
      D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 1.0f, 1.0f ),

      // Left Quad
      D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 0.0f, 0.0f ),
      D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 1.0f, 0.0f ),
      D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 0.0f, 1.0f ),
      D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 1.0f, 1.0f ),

      // Right Quad
      D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 0.0f, 0.0f ),
      D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 1.0f, 0.0f ),
      D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 0.0f, 1.0f ),
      D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 1.0f, 1.0f ),

      // Top Quad
      D3DSkyBoxVertex( -10.0f,  10.0f, -10.0f, 0.0f, 0.0f ),
      D3DSkyBoxVertex(  10.0f,  10.0f, -10.0f, 1.0f, 0.0f ),
      D3DSkyBoxVertex( -10.0f,  10.0f,  10.0f, 0.0f, 1.0f ),
      D3DSkyBoxVertex(  10.0f,  10.0f,  10.0f, 1.0f, 1.0f ),

      // Bottom Quad
      D3DSkyBoxVertex( -10.0f, -10.0f,  10.0f, 0.0f, 0.0f ),
      D3DSkyBoxVertex(  10.0f, -10.0f,  10.0f, 1.0f, 0.0f ),
      D3DSkyBoxVertex( -10.0f, -10.0f, -10.0f, 0.0f, 1.0f ),
      D3DSkyBoxVertex(  10.0f, -10.0f, -10.0f, 1.0f, 1.0f )
   };

   int vtxIdx = side * 4;
   IDirect3DVertexBuffer9* vb = m_renderer.createVertexBuffer(sizeof(D3DSkyBoxVertex) * 4, 
                                                              D3DUSAGE_WRITEONLY,
                                                              D3DSkyBoxVertex::FVF,
                                                              D3DPOOL_MANAGED);

   D3DSkyBoxVertex* pVertex = NULL;
   if (FAILED(vb->Lock(0, 0, (void**)&pVertex, D3DLOCK_DISCARD)))
   {
      throw std::logic_error(
         std::string("Cannot lock a vertex buffer"));
   }

   *pVertex++  = vertices[vtxIdx++];
   *pVertex++  = vertices[vtxIdx++];
   *pVertex++  = vertices[vtxIdx++];
   *pVertex++  = vertices[vtxIdx++];

   vb->Unlock();

   return new D3DSkyBoxSide(tex, m_d3Device, vb);

}

///////////////////////////////////////////////////////////////////////////////

Texture* D3DGraphicalEntitiesFactory::loadTexture(const std::string& fileName)
{
   IDirect3DTexture9* loadedTex = NULL;

   File* file = m_filesystem.open(fileName, std::ios_base::in | std::ios_base::binary);
   StreamBuffer<byte>* fileBuf = new StreamBuffer<byte>(file);

   D3DFORMAT texFormat = m_renderer.getOptimalTextureFormat();
   HRESULT res;

   res = D3DXCreateTextureFromFileInMemoryEx(&m_d3Device,
                                             fileBuf->getBuffer(), fileBuf->size(),
                                             D3DX_DEFAULT, D3DX_DEFAULT,
                                             D3DX_DEFAULT, 0, texFormat,
                                             D3DPOOL_MANAGED, 
                                             D3DX_DEFAULT, D3DX_DEFAULT,
                                             D3DCOLOR(), NULL, NULL, &loadedTex);

   delete fileBuf;

   if (FAILED(res))
   {
      std::string errorMsg = std::string("Can't load texture from file ") + fileName;
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

ParticleSystem* D3DGraphicalEntitiesFactory::createParticleSystem(
                                              const std::string& name, 
                                              Material& material,
                                              unsigned int particlesCount)
{
   return new D3DParticleSystem(m_d3Device, name, material, particlesCount);
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

PostProcessEffectRenderable* 
D3DGraphicalEntitiesFactory::createPostProcessEffectRenderable()
{
   return new D3DPostProcessEffectRenderable(m_d3Device, m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

GraphicalEffect* D3DGraphicalEntitiesFactory::createEffect(const std::string& name)
{
   ID3DXEffect* effect = NULL;
	ID3DXBuffer* errorsBuf = NULL;

   File* effectFile = m_filesystem.open(name, std::ios_base::in | std::ios_base::binary);
   StreamBuffer<char>* effectContents = new StreamBuffer<char>(effectFile);

	HRESULT res = D3DXCreateEffect(&m_d3Device, 
		                            effectContents->getBuffer(), 
                                  effectContents->size(),
		                            NULL, 
                                  NULL, 
                                  0, 
                                  NULL, 
                                  &effect, 
                                  &errorsBuf);
   delete  effectContents;

   if (FAILED(res) || (effect == NULL))
	{
      if (errorsBuf != NULL)
      {
         std::string compilationErrors = (const char*)errorsBuf->GetBufferPointer();
         throw std::runtime_error(std::string("Effect compilation error: ") + compilationErrors);
      }
      else
      {
         std::string errMsg = "Error while loading an effect: ";

         switch(res)
         {
         case D3DERR_INVALIDCALL:   errMsg += "System is not properly initialized"; break;
         case D3DXERR_INVALIDDATA:  errMsg += "Invalid data used"; break;
         case E_OUTOFMEMORY:        errMsg += "Out of memory"; break;
         default:                   errMsg += "Unknown error"; break;
         }

         throw std::runtime_error(errMsg);
      }
	}

   return new D3DGraphicalEffect(name, 
                                 m_d3Device, 
                                 m_renderer, 
                                 effect);
}

///////////////////////////////////////////////////////////////////////////////

Font* D3DGraphicalEntitiesFactory::createFont(const D3DXFONT_DESC& fontDesc)
{
   return new D3DFont(fontDesc, m_d3Device, m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

RendererImpl* D3DGraphicalEntitiesFactory::createFixedRendererImpl()
{
   return new D3DRendererImpl(m_d3Device, m_renderer.getMaxLightsCount());
}

///////////////////////////////////////////////////////////////////////////////

RenderingTargetsCleaner* D3DGraphicalEntitiesFactory::createTargetsCleaner()
{
   return new D3DRenderingTargetsCleaner(m_d3Device);
}

///////////////////////////////////////////////////////////////////////////////
