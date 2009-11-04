#include "core-TestFramework\TestFramework.h"
#include "core-AI\BTNode.h"
#include "core-AI\BTPrecondition.h"
#include "core-AI\BTSequenceNode.h"
#include "core-AI\BTSelectorNode.h"
#include "core-AI\BTNodeSelectionStrategy.h"
#include "core-AI\PreconditionSelectionStrategy.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class ActionMock : public BTNode
   {
   private:
      std::string m_msg;
      std::vector<std::string>& m_msgLog;
      ExecutionState m_state;

   public:
      ActionMock(const std::string& msg,
                 std::vector<std::string>& msgLog,
                 ExecutionState state = BT_COMPLETE)
      : m_msg(msg), m_msgLog(msgLog), m_state(state)
      {}

   protected:
      ExecutionState executeAction() 
      {
         m_msgLog.push_back(m_msg);
         return m_state;
      }

      void reset() {}
   };

   // -------------------------------------------------------------------------

   class FixedSelectMock : public BTNodeSelectionStrategy
   {
   private:
      int m_selectIdx;

   public:
      FixedSelectMock() : m_selectIdx(-1) {}

      void set(unsigned int idx) {m_selectIdx = idx;}

      unsigned int getNodeIdx(const std::vector<BTNode*>& nodes)
      {
         if ((m_selectIdx < 0) || (m_selectIdx >= nodes.size()))
         {
            return nodes.size() - 1;
         }
         else 
         {
            return m_selectIdx;
         }
      }
   };

   // -------------------------------------------------------------------------

   class PreconditionMock : public BTPrecondition
   {
   private:
      bool m_val;

   public:
      PreconditionMock(bool val) : m_val(val) {}

      bool check() {return m_val;}
   };

} // namespace anonymous


///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, sequence)
{
   std::vector<std::string> msgLog;

   BTSequenceNode sequence;
   sequence.add(new ActionMock("stand up", msgLog));
   sequence.add(new ActionMock("walk", msgLog));

   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, sequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("stand up"), msgLog[0]);

   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, sequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("walk"), msgLog[0]);

   // once complete, the sequence starts anew
   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, sequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("stand up"), msgLog[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, embeddedSequence)
{
   std::vector<std::string> msgLog;

   BTSequenceNode* goToSequence = new BTSequenceNode();
   goToSequence->add(new ActionMock("stand up", msgLog));
   goToSequence->add(new ActionMock("walk", msgLog));

   BTSequenceNode getFoodSequence;
   getFoodSequence.add(goToSequence);
   getFoodSequence.add(new ActionMock("take food", msgLog));

   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, getFoodSequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("stand up"), msgLog[0]);

   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, getFoodSequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("walk"), msgLog[0]);

   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, getFoodSequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("take food"), msgLog[0]);

   // once complete, the sequence starts anew
   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, getFoodSequence.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("stand up"), msgLog[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, failingSequence)
{
   std::vector<std::string> msgLog;

   BTSequenceNode shoot;
   shoot.add(new ActionMock("reload", msgLog, BT_FAILED));
   shoot.add(new ActionMock("fire", msgLog));

   CPPUNIT_ASSERT_EQUAL(BT_FAILED, shoot.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("reload"), msgLog[0]);

   // once complete, the sequence starts anew
   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_FAILED, shoot.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("reload"), msgLog[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, selector)
{
   std::vector<std::string> msgLog;

   FixedSelectMock* selStrategy = new FixedSelectMock();
   BTSelectorNode idle(selStrategy);
   idle.add(new ActionMock("smoke", msgLog));
   idle.add(new ActionMock("chat", msgLog));

   selStrategy->set(-1);
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, idle.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("chat"), msgLog[0]);

   selStrategy->set(0);
   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, idle.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("smoke"), msgLog[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, choosingProperSubtree)
{
   std::vector<std::string> msgLog;

   BTSelectorNode approach(new PreconditionSelectionStrategy());

   ActionMock* sneak = new ActionMock("sneak", msgLog);
   ActionMock* walk = new ActionMock("walk", msgLog);
   approach.add(sneak);
   approach.add(walk);

   sneak->setPrecondition(new PreconditionMock(false));
   walk->setPrecondition(new PreconditionMock(true));
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, approach.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("walk"), msgLog[0]);

   msgLog.clear();
   sneak->setPrecondition(new PreconditionMock(true));
   walk->setPrecondition(new PreconditionMock(false));
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, approach.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("sneak"), msgLog[0]);

   msgLog.clear();
   sneak->setPrecondition(new PreconditionMock(false));
   walk->setPrecondition(new PreconditionMock(false));
   CPPUNIT_ASSERT_EQUAL(BT_FAILED, approach.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, msgLog.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, checkingPreconditionForSequence)
{
   std::vector<std::string> msgLog;

   BTSequenceNode killEnemy;

   ActionMock* reload = new ActionMock("reload", msgLog);
   ActionMock* fire = new ActionMock("fire", msgLog);
   killEnemy.add(reload);
   killEnemy.add(fire);

   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, killEnemy.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("reload"), msgLog[0]);

   // sequence precondition is no longer valid
   killEnemy.setPrecondition(new PreconditionMock(false));

   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_FAILED, killEnemy.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, msgLog.size());

   // sequence precondition is valid again - but the sequence starts anew
   killEnemy.setPrecondition(new PreconditionMock(true));

   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_IN_PROGRESS, killEnemy.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("reload"), msgLog[0]);
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, checkingPreconditionForSelector)
{
   std::vector<std::string> msgLog;

   FixedSelectMock* selStrategy = new FixedSelectMock();
   BTSelectorNode idle(selStrategy);
   idle.add(new ActionMock("smoke", msgLog));
   idle.add(new ActionMock("chat", msgLog));

   selStrategy->set(-1);
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, idle.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, msgLog.size());
   CPPUNIT_ASSERT_EQUAL(std::string("chat"), msgLog[0]);

   // selector precondition changes
   idle.setPrecondition(new PreconditionMock(false));
   msgLog.clear();
   CPPUNIT_ASSERT_EQUAL(BT_FAILED, idle.execute());
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, msgLog.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(BehaviorTree, checkingPreconditionForLeafNode)
{
   std::vector<std::string> msgLog;

   ActionMock fire("fire", msgLog);
   CPPUNIT_ASSERT_EQUAL(BT_COMPLETE, fire.execute());

   // we run out of amunition
   fire.setPrecondition(new PreconditionMock(false));
   CPPUNIT_ASSERT_EQUAL(BT_FAILED, fire.execute());
}

///////////////////////////////////////////////////////////////////////////////
