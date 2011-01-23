#include "core-TestFramework\TestFramework.h"
#include "core\Enum.h"


///////////////////////////////////////////////////////////////////////////////

enum WeekEnd
{
   Sunday = 1,
   Saturday = 7
};
BEGIN_ENUM( WeekEnd );
   ENUM_VAL( Sunday );
   ENUM_VAL( Saturday );
END_ENUM( WeekEnd );

// -------------------------------------------------------------------------

enum WeekDays
{
   Monday,
   Tuesday,
   Wensday,
   Thursday,
   Friday
};
BEGIN_ENUM( WeekDays );
   ENUM_VAL( Monday );
   ENUM_VAL( Tuesday );
   ENUM_VAL( Wensday );
   ENUM_VAL( Thursday );
   ENUM_VAL( Friday );
END_ENUM( WeekDays );


///////////////////////////////////////////////////////////////////////////////

TEST( Enum, simpleEnum )
{
   const std::string &str = TEnum< WeekEnd >::from( Saturday );
   CPPUNIT_ASSERT_EQUAL( std::string( "Saturday" ), str );

   WeekEnd w;
   TEnum< WeekEnd >::to( "Sunday", w );
   CPPUNIT_ASSERT_EQUAL( w, Sunday );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Enum, valuesEnumeration )
{
   std::vector< std::string > values;

   TEnum< WeekEnd > weekendEnum;
   weekendEnum.getEnumerators( values );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)2, values.size() );
   CPPUNIT_ASSERT_EQUAL( std::string( "Sunday" ), values[0] );
   CPPUNIT_ASSERT_EQUAL( std::string( "Saturday" ), values[1] );

   values.clear();
   TEnum< WeekDays > weekdaysEnum;
   weekdaysEnum.getEnumerators( values );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)5, values.size() );
   CPPUNIT_ASSERT_EQUAL( std::string( "Monday" ), values[0] );
   CPPUNIT_ASSERT_EQUAL( std::string( "Tuesday" ), values[1] );
   CPPUNIT_ASSERT_EQUAL( std::string( "Wensday" ), values[2] );
   CPPUNIT_ASSERT_EQUAL( std::string( "Thursday" ), values[3] );
   CPPUNIT_ASSERT_EQUAL( std::string( "Friday" ), values[4] );
}

///////////////////////////////////////////////////////////////////////////////
