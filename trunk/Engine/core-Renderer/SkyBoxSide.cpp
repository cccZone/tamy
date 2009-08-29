#include "core-Renderer\SkyBoxSide.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\MaterialStage.h"
#include "core-Renderer\MaterialOperation.h"
#include "core-Renderer\CoordinatesOperation.h"
#include "core-Renderer\Texture.h"


///////////////////////////////////////////////////////////////////////////////

SkyBoxSide::SkyBoxSide(Texture* texture)
: m_texture(texture)
, m_material(NULL)
, m_renderingMatrices(1)
{
   m_material = new Material("skyBoxSideMat");
   MaterialStage* stage = new MaterialStage(*m_texture,
                   new MaterialOperation(MOP_SELECT_ARG1, SC_TEXTURE, SC_NONE),
                   new MaterialOperation(MOP_DISABLE, SC_NONE, SC_NONE),
                   new CoordinatesOperation(CC_CLAMP));
   m_material->addStage(stage);

   D3DXMatrixIdentity(&m_globalMtx);
   m_renderingMatrices.push_back(m_globalMtx);
}

///////////////////////////////////////////////////////////////////////////////

SkyBoxSide::~SkyBoxSide()
{
   delete m_material;
   m_material = NULL;

   delete m_texture;
   m_texture = NULL;
}

///////////////////////////////////////////////////////////////////////////////

const Array<D3DXMATRIX>& SkyBoxSide::getRenderingMatrices()
{
   return m_renderingMatrices;
}

///////////////////////////////////////////////////////////////////////////////

const Material& SkyBoxSide::getMaterial() const
{
   return *m_material;
}

///////////////////////////////////////////////////////////////////////////////

const D3DXMATRIX& SkyBoxSide::getGlobalTransform()
{
   return m_globalMtx;
}

///////////////////////////////////////////////////////////////////////////////

void SkyBoxSide::setGlobalMtx(const D3DXMATRIX& mtx)
{
   m_renderingMatrices[0] = mtx;
}

///////////////////////////////////////////////////////////////////////////////
