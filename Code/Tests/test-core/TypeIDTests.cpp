#include "core-TestFramework\TestFramework.h"
#include "core\TypeID.h"
#include "core\ClassTemplate.h"
#include "core\Class.h"

///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class ClassA {};

   template< typename B >
   class ClassT {};

   namespace dupa
   {
      class ClassB 
      {
      public:
         class ClassC {};
      };
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( TypeID, names )
{
   CPPUNIT_ASSERT_EQUAL( std::string( "::ClassA" ), TypeID< ClassA >().name() );
   CPPUNIT_ASSERT_EQUAL( std::string( "::ClassT< ::ClassA >" ), TypeID< ClassT< ClassA > >().name() );
   CPPUNIT_ASSERT_EQUAL( std::string( "::dupa::ClassB" ), TypeID< dupa::ClassB >().name() );
   CPPUNIT_ASSERT_EQUAL( std::string( "::dupa::ClassB::ClassC" ), TypeID< dupa::ClassB::ClassC >().name() );
   CPPUNIT_ASSERT_EQUAL( std::string( "::ClassT< ::dupa::ClassB >" ), TypeID< ClassT< dupa::ClassB > >().name() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Class, short )
{
   ClassTemplate temp1( 0, "::Namespace::ClassA", NULL );
   ClassTemplate temp2( 0, "::Namespace::ClassA< ClassB >", NULL );
   ClassTemplate temp3( 0, "ClassB", NULL );

   Class class1( temp1 );
   Class class2( temp2 );
   Class class3( temp3 );
   CPPUNIT_ASSERT_EQUAL( std::string( "ClassA" ), class1.getShortName() );
   CPPUNIT_ASSERT_EQUAL( std::string( "ClassA" ), class2.getShortName() );
   CPPUNIT_ASSERT_EQUAL( std::string( "ClassB" ), class3.getShortName() );
}

///////////////////////////////////////////////////////////////////////////////
