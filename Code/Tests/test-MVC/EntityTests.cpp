#include "core-TestFramework\TestFramework.h"
#include "core-MVC\Entity.h"
#include <d3dx9.h>



///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class ClonableEntityAMock : public Entity
   {
      DECLARE_ALLOCATOR( ClonableEntityAMock, AM_DEFAULT );
      DECLARE_CLASS()

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
   BEGIN_OBJECT( ClonableEntityAMock );
      PARENT( Entity );
      PROPERTY_EDIT("index", int, m_index);
   END_OBJECT();

   // -------------------------------------------------------------------------

   class ClonableEntityBMock : public Entity
   {
      DECLARE_ALLOCATOR( ClonableEntityBMock, AM_DEFAULT );
      DECLARE_CLASS()

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
   BEGIN_OBJECT( ClonableEntityBMock );
      PARENT( Entity );
      PROPERTY_EDIT("index", int, m_index);
   END_OBJECT();
}

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( ClonableEntityAMock );
DEFINE_TYPE_ID( ClonableEntityBMock );

///////////////////////////////////////////////////////////////////////////////

TEST( Entity, cloningSimple )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.clear();
   typesRegistry.addSerializableType< Entity >( "Entity", new TSerializableTypeInstantiator< Entity >() ); 
   typesRegistry.addSerializableType< ClonableEntityAMock >( "ClonableEntityAMock", new TSerializableTypeInstantiator< ClonableEntityAMock >() ); 

   Entity* entity = new ClonableEntityAMock( 5 );
   Entity* clonedEntity = entity->clone();

   CPPUNIT_ASSERT_EQUAL( std::string( "ClonableEntityAMock" ), clonedEntity->getVirtualRTTI().m_name );

   ClonableEntityAMock* castClonedEntity = DynamicCast< ClonableEntityAMock >( clonedEntity );
   CPPUNIT_ASSERT_EQUAL( 5, castClonedEntity->getIndex() );

   // cleanup
   delete clonedEntity;
   delete entity;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Entity, cloningHierarchy )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.clear();
   typesRegistry.addSerializableType< Entity >( "Entity", new TSerializableTypeInstantiator< Entity >() ); 
   typesRegistry.addSerializableType< ClonableEntityAMock >( "ClonableEntityAMock", new TSerializableTypeInstantiator< ClonableEntityAMock >() ); 
   typesRegistry.addSerializableType< ClonableEntityBMock >( "ClonableEntityBMock", new TSerializableTypeInstantiator< ClonableEntityBMock >() ); 

   Entity* rootEntity = new ClonableEntityAMock( 1 );
   Entity* firstChildEntity = new ClonableEntityBMock( 2 );
   Entity* secondChildEntity = new ClonableEntityAMock( 3 );

   rootEntity->add( firstChildEntity );
   firstChildEntity->add( secondChildEntity );

   Entity* clonedRootEntity = rootEntity->clone();

   CPPUNIT_ASSERT_EQUAL( std::string( "ClonableEntityAMock" ), clonedRootEntity->getVirtualRTTI().m_name );
   ClonableEntityAMock* castClonedRootEntity = DynamicCast< ClonableEntityAMock >( clonedRootEntity );
   CPPUNIT_ASSERT_EQUAL( 1, castClonedRootEntity->getIndex() );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, clonedRootEntity->getEntityChildren().size() );

   Entity* clonedFirstChildEntity = clonedRootEntity->getEntityChildren()[0];
   CPPUNIT_ASSERT_EQUAL( std::string( "ClonableEntityBMock" ), clonedFirstChildEntity->getVirtualRTTI().m_name );
   ClonableEntityBMock* castClonedFirstChildEntity = DynamicCast< ClonableEntityBMock >( clonedFirstChildEntity );
   CPPUNIT_ASSERT_EQUAL( 2, castClonedFirstChildEntity->getIndex() );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, clonedFirstChildEntity->getEntityChildren().size() );

   Entity* clonedSecondChildEntity = clonedFirstChildEntity->getEntityChildren()[0];
   CPPUNIT_ASSERT_EQUAL( std::string( "ClonableEntityAMock" ), clonedSecondChildEntity->getVirtualRTTI().m_name );
   ClonableEntityAMock* castClonedSecondChildEntity = DynamicCast< ClonableEntityAMock >( clonedSecondChildEntity );
   CPPUNIT_ASSERT_EQUAL( 3, castClonedSecondChildEntity->getIndex() );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, clonedSecondChildEntity->getEntityChildren().size() );

   // cleanup
   delete rootEntity;
   delete clonedRootEntity;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////
