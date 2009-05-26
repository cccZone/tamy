#include "core-TestFramework\TestFramework.h"
#include "GraphicalNodeMock.h"
#include "core\BoundingSphere.h"
#include "core\MatrixWriter.h"
#include "GraphicalNodeMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(AbstractGraphicalNode, boundingSpheres)
{
   float boundingSphereRadius = 1;
   GraphicalNodeMock node;
   node.setBoundingSphereRadius(1);

   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0), node.getBoundingSphere().origin);
   CPPUNIT_ASSERT_EQUAL(1.f, node.getBoundingSphere().radius);

   D3DXMatrixTranslation(&(node.accessLocalMtx()), 10, 20, 30);

   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(10, 20, 30), node.getBoundingSphere().origin);
   CPPUNIT_ASSERT_EQUAL(1.f, node.getBoundingSphere().radius);

   node.setBoundingSphereRadius(2);

   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(10, 20, 30), node.getBoundingSphere().origin);
   CPPUNIT_ASSERT_EQUAL(2.f, node.getBoundingSphere().radius);

   D3DXMatrixScaling(&(node.accessLocalMtx()), 2, 3, 4);

   CPPUNIT_ASSERT_EQUAL(D3DXVECTOR3(0, 0, 0), node.getBoundingSphere().origin);
   CPPUNIT_ASSERT_EQUAL(2.f, node.getBoundingSphere().radius);
}

///////////////////////////////////////////////////////////////////////////////
