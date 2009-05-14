#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\MaterialOperation.h"
#include "MaterialOperationImplementationMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST(MaterialOperation, multiply)
{
   MaterialOperationImplementationMock impl;

   MaterialOperation op(impl, MOP_MULTIPLY, SC_LRP, SC_TEXTURE);

   op.setForRendering(0);
   CPPUNIT_ASSERT_EQUAL(std::string("lrp"), impl.getSource(0));
   CPPUNIT_ASSERT_EQUAL(std::string("tex"), impl.getSource(1));
   CPPUNIT_ASSERT_EQUAL(std::string("multiply"), impl.getOperation());
}

///////////////////////////////////////////////////////////////////////////////
