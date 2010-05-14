#include "core-TestFramework\TestFramework.h"
#include "core\TypeID.h"


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
