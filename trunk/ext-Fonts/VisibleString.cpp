#include "ext-Fonts\VisibleString.h"
#include "core\NodeVisitor.h"
#include "core\TNodesVisitor.h"
#include "ext-Fonts\Font.h"
#include "core-Renderer\AbstractGraphicalEntity.h"
#include "core-Renderer\GraphicalEntityInstantiator.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////

VisibleString::VisibleString(Font& font)
      : m_font(font)
{
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
}

///////////////////////////////////////////////////////////////////////////////
