#include "core-TestFramework\TestFramework.h"
#include "core-MVC\Entity.h"
#include <d3dx9.h>



///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class ClonableEntityAMock : public Entity
   {
      DECLARE_CLASS( ClonableEntityAMock )

   private:
      int m_index;

   public:
      ClonableEntityAMock() : m_index( -1 ) {}
      ClonableEntityAMock( int idx ) : m_index( idx ) {}

      int getIndex() const { return m_index; }

   protected:
      Entity* cloneSelf() const
      {
         return new ClonableEntityAMock( m_index );
      }
   };
   BEGIN_OBJECT( ClonableEntityAMock, Entity )
      PROPERTY_EDIT("index", int, m_index)
   END_OBJECT()
   REGISTER_RTTI( ClonableEntityAMock );

   // -------------------------------------------------------------------------

   class ClonableEntityBMock : public Entity
   {
      DECLARE_CLASS( ClonableEntityBMock )

   private:
      int m_index;

   public:
      ClonableEntityBMock() : m_index( -1 ) {}
      ClonableEntityBMock( int idx ) : m_index( idx ) {}

      int getIndex() const { return m_index; }

   protected:
      Entity* cloneSelf() const
      {
         return new ClonableEntityBMock( m_index );
      }
   };
   BEGIN_OBJECT( ClonableEntityBMock, Entity )
      PROPERTY_EDIT("index", int, m_index)
   END_OBJECT()
   REGISTER_RTTI( ClonableEntityBMock );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Entity, cloningSimple )
{
   Entity* entity = new ClonableEntityAMock( 5 );
   Entity* clonedEntity = entity->clone();

   CPPUNIT_ASSERT_EQUAL( std::string( "::ClonableEntityAMock" ), clonedEntity->getVirtualClass().getName() );

   ClonableEntityAMock* castClonedEntity = DynamicCast< ClonableEntityAMock >( clonedEntity );
   CPPUNIT_ASSERT_EQUAL( 5, castClonedEntity->getIndex() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( Entity, cloningHierarchy )
{
   Entity* rootEntity = new ClonableEntityAMock( 1 );
   Entity* firstChildEntity = new ClonableEntityBMock( 2 );
   Entity* secondChildEntity = new ClonableEntityAMock( 3 );

   rootEntity->add( firstChildEntity );
   firstChildEntity->add( secondChildEntity );

   Entity* clonedRootEntity = rootEntity->clone();

   CPPUNIT_ASSERT_EQUAL( std::string( "::ClonableEntityAMock" ), clonedRootEntity->getVirtualClass().getName() );
   ClonableEntityAMock* castClonedRootEntity = DynamicCast< ClonableEntityAMock >( clonedRootEntity );
   CPPUNIT_ASSERT_EQUAL( 1, castClonedRootEntity->getIndex() );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, clonedRootEntity->getEntityChildren().size() );

   Entity* clonedFirstChildEntity = clonedRootEntity->getEntityChildren()[0];
   CPPUNIT_ASSERT_EQUAL( std::string( "::ClonableEntityBMock" ), clonedFirstChildEntity->getVirtualClass().getName() );
   ClonableEntityBMock* castClonedFirstChildEntity = DynamicCast< ClonableEntityBMock >( clonedFirstChildEntity );
   CPPUNIT_ASSERT_EQUAL( 2, castClonedFirstChildEntity->getIndex() );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, clonedFirstChildEntity->getEntityChildren().size() );

   Entity* clonedSecondChildEntity = clonedFirstChildEntity->getEntityChildren()[0];
   CPPUNIT_ASSERT_EQUAL( std::string( "::ClonableEntityAMock" ), clonedSecondChildEntity->getVirtualClass().getName() );
   ClonableEntityAMock* castClonedSecondChildEntity = DynamicCast< ClonableEntityAMock >( clonedSecondChildEntity );
   CPPUNIT_ASSERT_EQUAL( 3, castClonedSecondChildEntity->getIndex() );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, clonedSecondChildEntity->getEntityChildren().size() );
}

///////////////////////////////////////////////////////////////////////////////
