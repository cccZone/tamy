#include "core-Renderer\SkyBox.h"
#include "core-Renderer\Material.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"


///////////////////////////////////////////////////////////////////////////////

SkyBox::SkyBox()
      : Node("SkyBox")
{
   for (unsigned char i = 0; i < 6; ++i)
   {
      m_materials[i] = NULL;
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
   startRendering();

   for (unsigned char i = 0; i < 6; ++i)
   {
      if (m_materials[i] == NULL) continue;

      m_materials[i]->setForRendering();
      renderSide(static_cast<SkyBoxSides> (i));
   }

   endRendering();
}

///////////////////////////////////////////////////////////////////////////////

void SkyBox::setMaterial(SkyBoxSides side, Material& material)
{
   m_materials[side] = &material;
}

///////////////////////////////////////////////////////////////////////////////
