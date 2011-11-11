#include "core-TestFramework\TestFramework.h"
#include "core-AppFlow\TimeController.h"
#include "core-AppFlow\TimeDependent.h"
#include "UpdatableMock.h"
#include "SpeedChangeEventMock.h"


///////////////////////////////////////////////////////////////////////////////

TEST( TimeController, multipleTracks )
{
   TimeController controller;
   controller.add("track1");
   controller.add("track2");

   UpdatableMock updatable1;
   UpdatableMock updatable2;
   controller.get("track1").add( updatable1 );
   controller.get("track2").add( updatable2 );

   controller.update(1);
   CPPUNIT_ASSERT_EQUAL(1.f, updatable1.getLastTime());
   CPPUNIT_ASSERT_EQUAL(1.f, updatable2.getLastTime());

   controller.update(2);
   CPPUNIT_ASSERT_EQUAL(2.f, updatable1.getLastTime());
   CPPUNIT_ASSERT_EQUAL(2.f, updatable2.getLastTime());
};

///////////////////////////////////////////////////////////////////////////////

TEST(TimeControllerTrack, timeScaling)
{
   UpdatableMock updatable1;
   UpdatableMock updatable2;
   TimeControllerTrack track1("track1");
   TimeControllerTrack track2("track2");
   track1.add( updatable1 );
   track2.add( updatable2 );

   track1.update(1);
   track2.update(1);
   CPPUNIT_ASSERT_EQUAL(1.f, updatable1.getLastTime());
   CPPUNIT_ASSERT_EQUAL(1.f, updatable2.getLastTime());

   track1.setSpeed(0.5f);

   track1.update(1);
   track2.update(1);
   CPPUNIT_ASSERT_EQUAL(0.5f, updatable1.getLastTime());
   CPPUNIT_ASSERT_EQUAL(1.f, updatable2.getLastTime());

};

///////////////////////////////////////////////////////////////////////////////

TEST(TimeControllerTrack, framerateLimitation)
{
   UpdatableMock updatable;
   TimeControllerTrack track("track");
   track.add( updatable );

   // this limit means that the track can be updated maximally twice per second
   track.setLimit(2);

   // let's check it - first a single update per second...
   track.update(1);
   CPPUNIT_ASSERT_EQUAL(1, updatable.getTimesCalled());

   // ... two regular updates per second
   track.update(0.5f);
   track.update(0.5f);
   CPPUNIT_ASSERT_EQUAL(2, updatable.getTimesCalled());

   // ... four regular updates per second
   track.update(0.25f);
   track.update(0.25f);
   track.update(0.25f);
   track.update(0.25f);
   CPPUNIT_ASSERT_EQUAL(2, updatable.getTimesCalled());
   CPPUNIT_ASSERT_EQUAL(0.5f, updatable.getLastTime());

   // irregular updates...
   track.update(0.2f);
   CPPUNIT_ASSERT_EQUAL(0, updatable.getTimesCalled());

   track.update(0.4f);
   CPPUNIT_ASSERT_EQUAL(1, updatable.getTimesCalled());
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.6f, updatable.getLastTime(), 0.0001f);

   track.update(0.1f);
   CPPUNIT_ASSERT_EQUAL(0, updatable.getTimesCalled());

   track.update(0.2f);
   CPPUNIT_ASSERT_EQUAL(0, updatable.getTimesCalled());

   track.update(0.2f);
   CPPUNIT_ASSERT_EQUAL(1, updatable.getTimesCalled());
   CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5f, updatable.getLastTime(), 0.0001f);
};

///////////////////////////////////////////////////////////////////////////////
