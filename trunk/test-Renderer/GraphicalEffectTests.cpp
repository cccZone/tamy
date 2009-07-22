#include "core-TestFramework\TestFramework.h"
#include "core\Array.h"
#include "GraphicalNodeMock.h"
#include "GraphicalEffectMock.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

TEST(GraphicalEffect, settingSimpleArgument)
{
   std::vector<std::string> callSequenceVec;

   GraphicalEffectTests::GraphicalNodeMock node(callSequenceVec);
   Array<AbstractGraphicalNode*> nodes;
   nodes.push_back(&node);

   EffectDataSourceMock* dataSource = new EffectDataSourceMock(callSequenceVec);
   GraphicalEffectMock effect(dataSource, callSequenceVec);

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, callSequenceVec.size());

   effect.render(nodes, 0, 1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequenceVec.size());

   CPPUNIT_ASSERT_EQUAL(std::string("EffectDataSource::setData"), callSequenceVec[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::beginRendering"), callSequenceVec[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalNode::render"), callSequenceVec[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("GraphicalEffect::endRendering"), callSequenceVec[3]);
}

///////////////////////////////////////////////////////////////////////////////
