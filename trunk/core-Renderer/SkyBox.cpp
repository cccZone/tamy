#include "core-Renderer\SkyBox.h"
#include "core-Renderer\Texture.h"
#include "core-Renderer\StageTextureRenderer.h"
#include "core-Renderer\RenderingTargetsPolicy.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

SkyBox::SkyBox(StageTextureRenderer& textureRenderer, 
               RenderingTargetsPolicy& policy)
      : Node("SkyBox", false),
      m_textureRenderer(textureRenderer),
      m_policy(policy)
{
   for (unsigned char i = 0; i < 6; ++i)
   {
      m_textures[i] = NULL;
   }
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
      m_textureRenderer.setForRendering(0, *(m_textures[i]));

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
