#include "core-Renderer\SkyBox.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\MaterialImpl.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

SkyBox::SkyBox(MaterialImpl* materialImpl, 
               RenderingTargetsPolicy& policy)
      : Node("SkyBox", false),
      m_materialImpl(materialImpl),
      m_policy(policy)
{
   for (unsigned char i = 0; i < 6; ++i)
   {
      m_textures[i] = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

SkyBox::~SkyBox()
{
   delete m_materialImpl;
   m_materialImpl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SkyBox::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<SkyBox>);
}


///////////////////////////////////////////////////////////////////////////////

void SkyBox::render()
{
   m_policy.setTargets(0);
   startRendering();

   for (unsigned char i = 0; i < 6; ++i)
   {
      if (m_textures[i] == NULL) continue;
      m_materialImpl->setTexture(0, *(m_textures[i]));

      renderSide(static_cast<SkyBoxSides> (i));
   }

   endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void SkyBox::setTexture(SkyBoxSides side, Texture& texture)
{
   m_textures[side] = &(texture.getImpl());
}

///////////////////////////////////////////////////////////////////////////////
