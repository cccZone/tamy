#include "core-TestFramework\TestFramework.h"
#include "core\ReflectionObject.h"
#include "core\RefPtr.h"
#include "core\InArrayStream.h"
#include "core\OutArrayStream.h"


///////////////////////////////////////////////////////////////////////////////

namespace 
{
   struct TestObject : public ReflectionObject
   {
      int m_val;

      TestObject( int val = 7 ) : m_val( val ) {}
   };

   // -------------------------------------------------------------------------

   struct ObjWithRefPtrMembers : public ReflectionObject
   {
      TRefPtr< TestObject >        m_obj1;
      TRefPtr< TestObject >        m_obj2;

      ObjWithRefPtrMembers( TestObject* obj1 = NULL, TestObject* obj2 = NULL )
         : m_obj1( obj1 )
         , m_obj2( obj2 )
      {
      }
   };
}

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( TestObject );
DEFINE_TYPE_ID( ObjWithRefPtrMembers );

///////////////////////////////////////////////////////////////////////////////

TEST( RefPtr, basicReferencesCounting )
{
   TestObject managedObject;

   CPPUNIT_ASSERT_EQUAL( 1, managedObject.getReferencesCount() );

   {
      TRefPtr<TestObject> ptr1( &managedObject );
      CPPUNIT_ASSERT_EQUAL( 2, managedObject.getReferencesCount() );

      {
         TRefPtr<TestObject> copiedRefPtr( ptr1 );
         CPPUNIT_ASSERT_EQUAL( 3, managedObject.getReferencesCount() );

         {
            TRefPtr<TestObject> assignedRefPtr_1;
            assignedRefPtr_1 = ptr1;
            CPPUNIT_ASSERT_EQUAL( 4, managedObject.getReferencesCount() );

            {
               TRefPtr<TestObject> assignedRefPtr_2;
               assignedRefPtr_2 = &managedObject;
               CPPUNIT_ASSERT_EQUAL( 5, managedObject.getReferencesCount() );
            }

            CPPUNIT_ASSERT_EQUAL( 4, managedObject.getReferencesCount() );
         }

         CPPUNIT_ASSERT_EQUAL( 3, managedObject.getReferencesCount() );
      }

      CPPUNIT_ASSERT_EQUAL( 2, managedObject.getReferencesCount() );
   }

   CPPUNIT_ASSERT_EQUAL( 1, managedObject.getReferencesCount() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( RefPtr, classMember )
{
   TestObject obj1;
   TestObject obj2;

   CPPUNIT_ASSERT_EQUAL( 1, obj1.getReferencesCount() );
   CPPUNIT_ASSERT_EQUAL( 1, obj2.getReferencesCount() );
   {
      ObjWithRefPtrMembers objWithMembers( &obj1, &obj2 );

      CPPUNIT_ASSERT_EQUAL( 2, obj1.getReferencesCount() );
      CPPUNIT_ASSERT_EQUAL( 2, obj2.getReferencesCount() );
      {
         ObjWithRefPtrMembers copiedObjWithMembers( objWithMembers );

         CPPUNIT_ASSERT_EQUAL( 3, obj1.getReferencesCount() );
         CPPUNIT_ASSERT_EQUAL( 3, obj2.getReferencesCount() );
      }

      CPPUNIT_ASSERT_EQUAL( 2, obj1.getReferencesCount() );
      CPPUNIT_ASSERT_EQUAL( 2, obj2.getReferencesCount() );
   }

   CPPUNIT_ASSERT_EQUAL( 1, obj1.getReferencesCount() );
   CPPUNIT_ASSERT_EQUAL( 1, obj2.getReferencesCount() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( RefPtr, dataAccess )
{
   TestObject obj (7 );

   TRefPtr< TestObject > ptr( &obj );
   CPPUNIT_ASSERT_EQUAL( 7, ptr->m_val );
}

///////////////////////////////////////////////////////////////////////////////
