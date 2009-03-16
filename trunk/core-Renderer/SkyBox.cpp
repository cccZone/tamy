#include "core-Renderer\SkyBox.h"
#include "core-Renderer\Material.h"


///////////////////////////////////////////////////////////////////////////////

SkyBox::SkyBox()
{
   for (unsigned char i = 0; i < 6; ++i)
   {
      m_materials[i] = NULL;
   }
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
