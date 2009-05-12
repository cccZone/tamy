#include "core-TestFramework\TestFramework.h"
#include "core-AI\Agent.h"
#include "Enemy.h"
#include "Rambo.h"
#include "core-AI\MessageDispatcher.h"
#include "DemolitionMan.h"
#include "Grunt.h"
#include "ExplosiveCharge.h"

///////////////////////////////////////////////////////////////////////////////

TEST(Usablility, ramboScenario)
{
   MessageDispatcher msgDispatcher;
   Enemy enemy;
   Rambo rambo;
   enemy.setMessageDispatcher(msgDispatcher);
   rambo.setMessageDispatcher(msgDispatcher);

   CPPUNIT_ASSERT_EQUAL(std::string("looking for an ass to kick"), rambo.getAction());

   rambo.seesEnemy(enemy);
   CPPUNIT_ASSERT_EQUAL(std::string("looking for an ass to kick"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(10, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(2, enemy.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("kickin' ass"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(10, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(2, enemy.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("kickin' ass"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(9, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(1, enemy.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("kickin' ass"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(8, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(0, enemy.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("looking for an ass to kick"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(8, rambo.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("healing"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(8, rambo.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("healing"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(9, rambo.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("looking for an ass to kick"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(10, rambo.getHealth());

   enemy.revive();
   enemy.startShooting(rambo);

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("looking for an ass to kick"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(10, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(2, enemy.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("kickin' ass"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(10, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(2, enemy.getHealth());

   rambo.update();
   enemy.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("kickin' ass"), rambo.getAction());
   CPPUNIT_ASSERT_EQUAL(9, rambo.getHealth());
   CPPUNIT_ASSERT_EQUAL(1, enemy.getHealth());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Usablility, specialistScenario)
{
   MessageDispatcher msgDispatcher;

   DemolitionMan specialist;
   Grunt grunt;
   ExplosiveCharge charge;
   specialist.setMessageDispatcher(msgDispatcher);
   grunt.setMessageDispatcher(msgDispatcher);
   charge.setMessageDispatcher(msgDispatcher);

   CPPUNIT_ASSERT_EQUAL(std::string(""), specialist.getAction());
   CPPUNIT_ASSERT_EQUAL(std::string(""), grunt.getAction());
   CPPUNIT_ASSERT_EQUAL(false, charge.isArmed());

   charge.armMe(grunt);
   specialist.update();
   grunt.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string(""), specialist.getAction());
   CPPUNIT_ASSERT_EQUAL(std::string(""), grunt.getAction());
   CPPUNIT_ASSERT_EQUAL(false, charge.isArmed());

   charge.armMe(specialist);
   specialist.update();
   grunt.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("arming an explosive charge"), specialist.getAction());
   CPPUNIT_ASSERT_EQUAL(std::string(""), grunt.getAction());
   CPPUNIT_ASSERT_EQUAL(false, charge.isArmed());

   specialist.update();
   grunt.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string("arming an explosive charge"), specialist.getAction());
   CPPUNIT_ASSERT_EQUAL(std::string(""), grunt.getAction());
   CPPUNIT_ASSERT_EQUAL(true, charge.isArmed());

   specialist.update();
   grunt.update();
   msgDispatcher.update(1);
   CPPUNIT_ASSERT_EQUAL(std::string(""), specialist.getAction());
   CPPUNIT_ASSERT_EQUAL(std::string(""), grunt.getAction());
   CPPUNIT_ASSERT_EQUAL(true, charge.isArmed());
}

///////////////////////////////////////////////////////////////////////////////

