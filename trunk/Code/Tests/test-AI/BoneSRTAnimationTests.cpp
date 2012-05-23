#include "core-TestFramework\TestFramework.h"
#include "core-AI/BoneSRTAnimation.h"


///////////////////////////////////////////////////////////////////////////////

TEST( BoneSRTAnimation, duration )
{
   BoneSRTAnimation anim;
   anim.addTranslationKey( 1.0f, Vector::ZERO );
   anim.addOrientationKey( 2.0f, Quaternion( 0, 0, 0, 0 ) );

   CPPUNIT_ASSERT_EQUAL( 2.0f, anim.getDuration() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( BoneSRTAnimation, simpleTranslationAnim )
{
   BoneSRTAnimation anim;
   anim.addTranslationKey( 0, Vector::ZERO );
   anim.addTranslationKey( 1, Vector::OX );

   CPPUNIT_ASSERT_EQUAL( 1.0f, anim.getDuration() );

   BoneSRTAnimationPlayer player( anim );

   Vector result;
   CPPUNIT_ASSERT( player.getTranslation( 0.0f, result ) );
   COMPARE_VEC( Vector( 0, 0, 0 ), result );

   CPPUNIT_ASSERT( player.getTranslation( 1.0f, result ) );
   COMPARE_VEC( Vector( 1, 0, 0 ), result );

   CPPUNIT_ASSERT( player.getTranslation( 0.5f, result ) );
   COMPARE_VEC( Vector( 0.5f, 0, 0 ), result );

   // outside the boundaries access - return the boundary values, don't perform any looped lookups
   CPPUNIT_ASSERT( player.getTranslation( -0.1f, result ) );
   COMPARE_VEC( Vector( 0, 0, 0 ), result );

   CPPUNIT_ASSERT( player.getTranslation( 1.1f, result ) );
   COMPARE_VEC( Vector( 1, 0, 0 ), result );
}

///////////////////////////////////////////////////////////////////////////////

TEST( BoneSRTAnimation, noKeysAnim )
{
   BoneSRTAnimation anim;

   CPPUNIT_ASSERT_EQUAL( 0.0f, anim.getDuration() );

   BoneSRTAnimationPlayer player( anim );

   Vector resultVec;
   Quaternion resultQuat;
   CPPUNIT_ASSERT_EQUAL( false, player.getTranslation( 0.0f, resultVec ) );
   CPPUNIT_ASSERT_EQUAL( false, player.getOrientation( 0.0f, resultQuat ) );
}

///////////////////////////////////////////////////////////////////////////////

TEST( BoneSRTAnimation, replacingAnimKeys )
{
   BoneSRTAnimation anim;
   anim.addTranslationKey( 0, Vector( 0, 0, 0 ) );
   anim.addTranslationKey( 1, Vector( 1, 0, 0 ) );
   anim.addTranslationKey( 1, Vector( 2, 0, 0 ) );

   CPPUNIT_ASSERT_EQUAL( 1.0f, anim.getDuration() );

   BoneSRTAnimationPlayer player( anim );

   Vector result;
   CPPUNIT_ASSERT( player.getTranslation( 0.0f, result ) );
   COMPARE_VEC( Vector( 0, 0, 0 ), result );

   CPPUNIT_ASSERT( player.getTranslation( 0.5f, result ) );
   COMPARE_VEC( Vector( 1.0f, 0, 0 ), result );

   CPPUNIT_ASSERT( player.getTranslation( 1.0f, result ) );
   COMPARE_VEC( Vector( 2.0f, 0, 0 ), result );
}

///////////////////////////////////////////////////////////////////////////////
