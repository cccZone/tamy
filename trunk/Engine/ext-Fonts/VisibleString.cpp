#include "ext-Fonts\VisibleString.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "ext-Fonts\Font.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include "core-Renderer\Color.h"
#include "core-Renderer\Material.h"
#include "core-Renderer\LightReflectingProperties.h"
#include "core-Renderer\MaterialReplacer.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

VisibleString::VisibleString(Font& font)
      : Node("visibleString"),
      m_font(font),
      m_material(NULL),
      m_matReplacer(NULL)
{
   const Material& fontMat = m_font.getMaterial();
   m_material = new Material(fontMat);

   m_matReplacer = new MaterialReplacer(fontMat, *m_material);
}

///////////////////////////////////////////////////////////////////////////////

VisibleString::~VisibleString()
{
   delete m_matReplacer;
   m_matReplacer = NULL;

   delete m_material;
   m_material = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void VisibleString::onAccept(NodeVisitor& visitor)
{
   REGISTER_NODE_VISITOR(TNodesVisitor<VisibleString>);
}

///////////////////////////////////////////////////////////////////////////////

void VisibleString::setText(const char* text)
{
   // clear current text
   while(getChildrenCount() > 0)
   {
      removeChild(*(getChildren().back()));
   }

   // create the new string representation
   float letterOffset = 0;
   unsigned int lettersCount = strlen(text);
   for (unsigned int i= 0; i < lettersCount; ++i)
   {
      char letter = text[i];

      AbstractGraphicalEntity& letterEnt = m_font.getChar(letter);

      GraphicalEntityInstantiator* letterNode = new GraphicalEntityInstantiator("letter");
      letterNode->attachEntity(letterEnt);
      D3DXMatrixTranslation(&(letterNode->accessLocalMtx()), letterOffset, 0, 0);
      
      addChild(letterNode);

      letterOffset += m_font.getCharWidth(letter);
   }

   // replace the original font material with our material
   this->accept(*m_matReplacer);
}

///////////////////////////////////////////////////////////////////////////////

void VisibleString::setColor(const Color& color)
{
   LightReflectingProperties& lrp = m_material->getLightReflectingProperties();
   lrp.setAmbientColor(color);
   lrp.setDiffuseColor(color);
}

///////////////////////////////////////////////////////////////////////////////

Material& VisibleString::getMaterial()
{
   return *m_material;
}

///////////////////////////////////////////////////////////////////////////////
