#include "core-TestFramework\TestFramework.h"
#include "core-AI\AISceneManager.h"
#include "core\CompositeSceneManager.h"
#include "Rambo.h"
#include "ExplosiveCharge.h"
#include "DemolitionMan.h"


///////////////////////////////////////////////////////////////////////////////

TEST(AISceneManager, updatingNodes)
{
   CompositeSceneManager mainSceneManager;
   AISceneManager* aiSceneManager = new AISceneManager();
   mainSceneManager.addSceneManager(aiSceneManager);

   Rambo* rambo = new Rambo();
   mainSceneManager.addNode(rambo);

   rambo->receiveWound(5);
   CPPUNIT_ASSERT_EQUAL(5, rambo->getHealth());

   rambo->heal();
   CPPUNIT_ASSERT_EQUAL(5, rambo->getHealth());

   aiSceneManager->update(0);
   CPPUNIT_ASSERT_EQUAL(6, rambo->getHealth());
}

///////////////////////////////////////////////////////////////////////////////

TEST(AISceneManager, makingSureEveryoneUsesTheSameMessagingSystem)
{
   CompositeSceneManager mainSceneManager;
   AISceneManager* aiSceneManager = new AISceneManager();
   mainSceneManager.addSceneManager(aiSceneManager);

   ExplosiveCharge* charge = new ExplosiveCharge();
   DemolitionMan* demoMan = new DemolitionMan();
   mainSceneManager.addNode(charge);
   mainSceneManager.addNode(demoMan);

   charge->armMe(*demoMan);
   CPPUNIT_ASSERT_EQUAL(std::string(""), demoMan->getAction());
   CPPUNIT_ASSERT_EQUAL(false, charge->isArmed());

   aiSceneManager->update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("arming an explosive charge"), demoMan->getAction());
   CPPUNIT_ASSERT_EQUAL(false, charge->isArmed());

   aiSceneManager->update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("arming an explosive charge"), demoMan->getAction());
   CPPUNIT_ASSERT_EQUAL(true, charge->isArmed());

   aiSceneManager->update(1);
   CPPUNIT_ASSERT_EQUAL(std::string(""), demoMan->getAction());
   CPPUNIT_ASSERT_EQUAL(true, charge->isArmed());
}

///////////////////////////////////////////////////////////////////////////////

