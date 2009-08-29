#include "core-TestFramework\TestFramework.h"
#include "ext-Fonts\VisibleString.h"
#include "core\MatrixWriter.h"
#include "FontMock.h"
#include <d3dx9.h>
#include "core-Renderer\AbstractGraphicalNode.h"


///////////////////////////////////////////////////////////////////////////////

TEST(VisibleString, settingText)
{
   FontMock font;
   VisibleString string(font);

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, string.getChildrenCount());

   string.setText("a");
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, string.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL('a', font.getRequestedChar(0));

   font.reset();
   string.setText("bcde");
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, string.getChildrenCount());
   CPPUNIT_ASSERT_EQUAL('b', font.getRequestedChar(0));
   CPPUNIT_ASSERT_EQUAL('c', font.getRequestedChar(1));
   CPPUNIT_ASSERT_EQUAL('d', font.getRequestedChar(2));
   CPPUNIT_ASSERT_EQUAL('e', font.getRequestedChar(3));
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisibleString, spacingBetweenLetters)
{
   FontMock font;
   VisibleString string(font);
   string.setText("ab");

   CPPUNIT_ASSERT_EQUAL((unsigned int)2, string.getChildrenCount());

   std::vector<D3DXMATRIX> expectedPos;
   expectedPos.push_back(D3DXMATRIX()); D3DXMatrixTranslation(&(expectedPos.back()), 0, 0, 0);
   expectedPos.push_back(D3DXMATRIX()); D3DXMatrixTranslation(&(expectedPos.back()), 10, 0, 0);

   const std::list<Node*>& children = string.getChildren();
   unsigned int i = 0;
   for (std::list<Node*>::const_iterator it = children.begin(); it != children.end(); ++it, ++i)
   {
      CPPUNIT_ASSERT_EQUAL(expectedPos[i], (*it)->getLocalMtx());
   }
}

///////////////////////////////////////////////////////////////////////////////

TEST(VisibleString, ownMaterialCopy)
{
   FontMock font;
   const Material& originalMat = font.getMaterial();

   VisibleString string(font);
   Material& instanceMat = string.getMaterial();

   // verify that each string uses its own material
   CPPUNIT_ASSERT(originalMat.getIndex() != instanceMat.getIndex());
}

///////////////////////////////////////////////////////////////////////////////
