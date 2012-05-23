#include "core-TestFramework\TestFramework.h"
#include "core\InArrayStream.h"
#include "core\OutArrayStream.h"
#include "core\ReflectionTypeComponent.h"
#include "core\ReflectionType.h"
#include "core\ReflectionTypesRegistry.h"
#include "core\ReflectionSaver.h"
#include "core\ReflectionLoader.h"
#include "core\ReflectionObject.h"
#include "core\ReflectionObjectsTracker.h"
#include "core\ReflectionPropertiesView.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   // -------------------------------------------------------------------------

   struct TestClass : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_val1;
      int      m_val2;

      TestClass( int val1 = 0, int val2 = 0, const char* id = NULL ) : ReflectionObject( id ), m_val1( val1 ), m_val2( val2 ) {}
      virtual ~TestClass() {}
   };
   BEGIN_OBJECT( TestClass );
      PROPERTY( int, m_val1 );
      PROPERTY( int, m_val2 );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct PatchedTestClass : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_val;

      PatchedTestClass( int val = 0 ) : m_val( val ) {}
      virtual ~PatchedTestClass() {}
   };
   BEGIN_OBJECT( PatchedTestClass );
      PATCH_MEMBER( m_val2, m_val );
      PROPERTY( int, m_val );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct DerivedTestClass : public TestClass
   {
      DECLARE_STRUCT()

      int      m_val3;

      DerivedTestClass( int val1 = 0, int val2 = 0, int val3 = 0 ) : TestClass( val1, val2 ), m_val3( val3 ) {}
   };
   BEGIN_OBJECT( DerivedTestClass );
      PARENT( TestClass );
      PROPERTY( int, m_val3 );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct PatchedDerivedTestClass : public PatchedTestClass
   {
      DECLARE_STRUCT()

      int      m_val2;

      PatchedDerivedTestClass( int val1 = 0, int val2 = 0 ) : PatchedTestClass( val1 ), m_val2( val2 ) {}
   };
   BEGIN_OBJECT( PatchedDerivedTestClass );
      PARENT( PatchedTestClass );
      PATCH_MEMBER( m_val3, m_val2 );
      PROPERTY( int, m_val2 );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct TestClassWithPointers : public ReflectionObject
   {
      DECLARE_STRUCT()

      int                           m_val;
      TestClassWithPointers*        m_ptr;

      TestClassWithPointers( int val = 0, TestClassWithPointers* ptr = NULL ) : m_val( val ), m_ptr( ptr ) {}
      ~TestClassWithPointers() { delete m_ptr; m_ptr = NULL; }
   };
   BEGIN_OBJECT( TestClassWithPointers );
      PROPERTY( int, m_val );
      PROPERTY( TestClassWithPointers*, m_ptr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct TestClassWithPODArray : public ReflectionObject
   {
      DECLARE_STRUCT()

      std::vector< int >                  m_arr;
   };
   BEGIN_OBJECT( TestClassWithPODArray );
      PROPERTY( std::vector< int >, m_arr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct TestClassWithPtrArray : public ReflectionObject
   {
      DECLARE_STRUCT()

      std::vector< TestClass* >           m_arr;

      ~TestClassWithPtrArray()
      {
         uint count = m_arr.size();
         for ( uint i = 0; i < count; ++i )
         {
            delete m_arr[i];
         }
         m_arr.clear();
      }
   };
   BEGIN_OBJECT( TestClassWithPtrArray );
      PROPERTY( std::vector< TestClass* >, m_arr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct TestClassWithSharedPointers : public ReflectionObject
   {
      DECLARE_STRUCT()

      TestClass*        m_ptr;

      TestClassWithSharedPointers( const char* id = NULL, TestClass* ptr = NULL ) : ReflectionObject( id ), m_ptr( ptr ) {}
      ~TestClassWithSharedPointers() { m_ptr = NULL; }
   };
   BEGIN_OBJECT( TestClassWithSharedPointers );
      PROPERTY( TestClassWithSharedPointers*, m_ptr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct TestClassNotifiable : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_valOrig;
      int      m_valPreSave;
      int      m_valPostLoad;

      TestClassNotifiable( int val = 0 ) : m_valOrig( val ), m_valPreSave( 0 ), m_valPostLoad( 0 ) {}
      virtual ~TestClassNotifiable() {}

      // ----------------------------------------------------------------------
      // ReflectionObject implementation
      // ----------------------------------------------------------------------
      void onObjectPreSave() { m_valPreSave = 1; }
      void onObjectLoaded() { m_valPostLoad = 1; }
   };
   BEGIN_OBJECT( TestClassNotifiable );
      PROPERTY( int, m_valOrig );
      PROPERTY( int, m_valPreSave );
      PROPERTY( int, m_valPostLoad );
   END_OBJECT();

   // -------------------------------------------------------------------------

   class ReflectionObjectsTrackerMock : public ReflectionObjectsTracker
   {
   private:
      std::vector< ReflectionObject* >       m_trackedObjects;

   public:
      ~ReflectionObjectsTrackerMock()
      {
         uint count = m_trackedObjects.size();
         for ( uint i = 0; i < count; ++i )
         {
            delete m_trackedObjects[i];
         }
         m_trackedObjects.clear();
      }

      // ----------------------------------------------------------------------
      // ReflectionObjectsTrackerMock implementation
      // ----------------------------------------------------------------------
      void trackInstance( ReflectionObject* object )
      {
         m_trackedObjects.push_back( object );
      }

      ReflectionObject* findInstance( const ReflectionObject::UniqueId& id )
      {
         uint count = m_trackedObjects.size();
         for ( uint i = 0; i < count; ++i )
         {
            if ( m_trackedObjects[i]->m_uniqueId == id )
            {
               return m_trackedObjects[i];
            }
         }

         return NULL;
      }
   };
}

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( TestClass )
DEFINE_TYPE_ID( PatchedTestClass )
DEFINE_TYPE_ID( TestClassNotifiable )
DEFINE_TYPE_ID( TestClassWithSharedPointers )
DEFINE_TYPE_ID( TestClassWithPtrArray )
DEFINE_TYPE_ID( TestClassWithPODArray )
DEFINE_TYPE_ID( TestClassWithPointers )
DEFINE_TYPE_ID( DerivedTestClass )
DEFINE_TYPE_ID( PatchedDerivedTestClass )

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, simpleTypes )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() ); 
   
   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the object
   TestClass obj( 5, 10 ); 
   saver.save( obj );
   saver.flush();

   // restore the object
   TestClass* restoredObject = loader.load< TestClass >();
   CPPUNIT_ASSERT( restoredObject != NULL );
   CPPUNIT_ASSERT_EQUAL( 5, restoredObject->m_val1 );
   CPPUNIT_ASSERT_EQUAL( 10, restoredObject->m_val2 );

   // cleanup
   delete restoredObject;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, patching )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   TestClass obj( 6, 11 ); 
   saver.save( obj );
   saver.flush();

   // clear the types registry
   typesRegistry.clear();

   // register the changed, "patched" type
   typesRegistry.addSerializableType< PatchedTestClass >( "PatchedTestClass", new TSerializableTypeInstantiator< PatchedTestClass >(), "TestClass" );

   // restore the object
   PatchedTestClass* restoredObject = loader.load< PatchedTestClass >();
   CPPUNIT_ASSERT( restoredObject != NULL );
   CPPUNIT_ASSERT_EQUAL( 11, restoredObject->m_val );

   // cleanup
   delete restoredObject;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, inheritance )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< DerivedTestClass >( "DerivedTestClass", new TSerializableTypeInstantiator< DerivedTestClass >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the object
   DerivedTestClass obj( 1, 4, 7 ); 
   saver.save( obj );
   saver.flush();

   // restore the object
   TestClass* restoredObjectBase = loader.load< TestClass >();
   CPPUNIT_ASSERT( restoredObjectBase != NULL );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObjectBase->m_val1 );
   CPPUNIT_ASSERT_EQUAL( 4, restoredObjectBase->m_val2 );

   DerivedTestClass* restoredObjectProper = static_cast< DerivedTestClass* >( restoredObjectBase );
   CPPUNIT_ASSERT_EQUAL( 7, restoredObjectProper->m_val3 );

   // cleanup
   delete restoredObjectBase;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, patching_inheritanceMissingOneType )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< DerivedTestClass >( "DerivedTestClass", new TSerializableTypeInstantiator< DerivedTestClass >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   DerivedTestClass obj( 3, 2, 8 ); 
   saver.save( obj );
   saver.flush();

   // clear the type definitions and define a new type that has an inheritance definition removed
   typesRegistry.clear();
   typesRegistry.addSerializableType< PatchedTestClass >( "PatchedTestClass", new TSerializableTypeInstantiator< PatchedTestClass >(), "TestClass" );
   typesRegistry.addSerializableType< PatchedDerivedTestClass >( "PatchedDerivedTestClass", new TSerializableTypeInstantiator< PatchedDerivedTestClass >(), "DerivedTestClass" );

   // restore the object
   PatchedTestClass* restoredObjectBase = loader.load< PatchedTestClass >();
   CPPUNIT_ASSERT( restoredObjectBase != NULL );
   CPPUNIT_ASSERT_EQUAL( 2, restoredObjectBase->m_val );

   PatchedDerivedTestClass* restoredObjectProper = static_cast< PatchedDerivedTestClass* >( restoredObjectBase );
   CPPUNIT_ASSERT_EQUAL( 8, restoredObjectProper->m_val2 );

   // cleanup
   delete restoredObjectBase;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, pointers )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClassWithPointers >( "TestClassWithPointers", new TSerializableTypeInstantiator< TestClassWithPointers >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   TestClassWithPointers* obj1 = new TestClassWithPointers( 1 );
   TestClassWithPointers obj2( 2, obj1 );
   saver.save( obj2 );
   saver.flush();

   // restore the object
   TestClassWithPointers* restoredObject = loader.load< TestClassWithPointers >();
   CPPUNIT_ASSERT( restoredObject != NULL );
   CPPUNIT_ASSERT_EQUAL( 2, restoredObject->m_val );

   CPPUNIT_ASSERT( restoredObject->m_ptr != NULL );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObject->m_ptr->m_val );

   // cleanup
   delete restoredObject;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, podsArrays )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClassWithPODArray >( "TestClassWithPODArray", new TSerializableTypeInstantiator< TestClassWithPODArray >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   TestClassWithPODArray obj;
   obj.m_arr.push_back( 1 );
   obj.m_arr.push_back( 2 );
   obj.m_arr.push_back( 3 );
   saver.save( obj );
   saver.flush();

   // restore the object
   TestClassWithPODArray* restoredObject = loader.load< TestClassWithPODArray >();
   CPPUNIT_ASSERT( restoredObject != NULL );
   CPPUNIT_ASSERT_EQUAL( (uint)3, restoredObject->m_arr.size() );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObject->m_arr[0] );
   CPPUNIT_ASSERT_EQUAL( 2, restoredObject->m_arr[1] );
   CPPUNIT_ASSERT_EQUAL( 3, restoredObject->m_arr[2] );

   // cleanup
   delete restoredObject;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, pointersArrays )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< TestClassWithPtrArray >( "TestClassWithPtrArray", new TSerializableTypeInstantiator< TestClassWithPtrArray >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   TestClassWithPtrArray obj;
   obj.m_arr.push_back( new TestClass( 1, 2 ) );
   obj.m_arr.push_back( new TestClass( 3, 4 ) );
   obj.m_arr.push_back( new TestClass( 5, 6 ) );
   saver.save( obj );
   saver.flush();

   // restore the object
   TestClassWithPtrArray* restoredObject = loader.load< TestClassWithPtrArray >();
   CPPUNIT_ASSERT( restoredObject != NULL );
   CPPUNIT_ASSERT_EQUAL( (uint)3, restoredObject->m_arr.size() );
   CPPUNIT_ASSERT( NULL != restoredObject->m_arr[0] );
   CPPUNIT_ASSERT( NULL != restoredObject->m_arr[1] );
   CPPUNIT_ASSERT( NULL != restoredObject->m_arr[2] );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObject->m_arr[0]->m_val1 );
   CPPUNIT_ASSERT_EQUAL( 2, restoredObject->m_arr[0]->m_val2 );
   CPPUNIT_ASSERT_EQUAL( 3, restoredObject->m_arr[1]->m_val1 );
   CPPUNIT_ASSERT_EQUAL( 4, restoredObject->m_arr[1]->m_val2 );
   CPPUNIT_ASSERT_EQUAL( 5, restoredObject->m_arr[2]->m_val1 );
   CPPUNIT_ASSERT_EQUAL( 6, restoredObject->m_arr[2]->m_val2 );

   // cleanup
   delete restoredObject;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, sharedPointers )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< TestClassWithSharedPointers >( "TestClassWithSharedPointers", new TSerializableTypeInstantiator< TestClassWithSharedPointers >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   TestClass* sharedObj = new TestClass( 1, 2 );
   TestClassWithSharedPointers obj1( "obj1", sharedObj );
   TestClassWithSharedPointers obj2( "obj1", sharedObj );
   saver.save( obj1 );
   saver.save( obj2 );
   saver.flush();

   // restore the object
   TestClassWithSharedPointers* restoredObject1 = loader.load< TestClassWithSharedPointers >();
   TestClassWithSharedPointers* restoredObject2 = loader.load< TestClassWithSharedPointers >();
   CPPUNIT_ASSERT( restoredObject1 != NULL );
   CPPUNIT_ASSERT( restoredObject2 != NULL );
   CPPUNIT_ASSERT( restoredObject1 != restoredObject2 );
   CPPUNIT_ASSERT( restoredObject1->m_ptr == restoredObject2->m_ptr );
   CPPUNIT_ASSERT( sharedObj != restoredObject1->m_ptr );

   // cleanup
   delete restoredObject1->m_ptr;
   delete restoredObject1;
   delete restoredObject2;
   delete sharedObj;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, instancesTracking )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< TestClassWithSharedPointers >( "TestClassWithSharedPointers", new TSerializableTypeInstantiator< TestClassWithSharedPointers >() );

   // set up an instances tracker
   ReflectionObjectsTrackerMock tracker;

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream, tracker );

   // serialize the objects - first one, than the other, so that their dependencies
   // are mapped independent of each other
   TestClass* sharedObj = new TestClass( 1, 2, "sharedObj" );
   TestClassWithSharedPointers obj1( "obj1", sharedObj );
   TestClassWithSharedPointers obj2( "obj2", sharedObj );
   saver.save( obj1 );
   saver.flush();
   saver.save( obj2 );
   saver.flush();

   // restore the object
   TestClassWithSharedPointers* restoredObject1 = loader.load< TestClassWithSharedPointers >();
   TestClassWithSharedPointers* restoredObject2 = loader.load< TestClassWithSharedPointers >();
   CPPUNIT_ASSERT( restoredObject1 != NULL );
   CPPUNIT_ASSERT( restoredObject2 != NULL );
   CPPUNIT_ASSERT( restoredObject1 != restoredObject2 );

   // let's see if the tracker managed to map the shared object - if it has,
   // both instances should have the same pointer, even though they were serialized in separate batches
   CPPUNIT_ASSERT( restoredObject1->m_ptr == restoredObject2->m_ptr );
   CPPUNIT_ASSERT( sharedObj != restoredObject1->m_ptr );

   // cleanup - and now we don't need to delete the restored objects ourselves - the tracker will take care of that
   delete sharedObj;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( RTTI, isExactlyA )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< DerivedTestClass >( "DerivedTestClass", new TSerializableTypeInstantiator< DerivedTestClass >() );

   DerivedTestClass instA;
   TestClass instB;

   CPPUNIT_ASSERT( instA.isExactlyA< DerivedTestClass >() );
   CPPUNIT_ASSERT( !instA.isExactlyA< TestClass >() );
   CPPUNIT_ASSERT( !instB.isExactlyA< DerivedTestClass >() );
   CPPUNIT_ASSERT( instB.isExactlyA< TestClass >() );

   CPPUNIT_ASSERT( instA.isExactlyA( SerializableReflectionType( "DerivedTestClass" ) ) );
   CPPUNIT_ASSERT( !instA.isExactlyA( SerializableReflectionType( "TestClass" ) ) );
   CPPUNIT_ASSERT( !instB.isExactlyA( SerializableReflectionType( "DerivedTestClass" ) ) );
   CPPUNIT_ASSERT( instB.isExactlyA( SerializableReflectionType( "TestClass" ) ) );

   // cleanup - and now we don't need to delete the restored objects ourselves - the tracker will take care of that
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( RTTI, isA )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< DerivedTestClass >( "DerivedTestClass", new TSerializableTypeInstantiator< DerivedTestClass >() );
   typesRegistry.addSerializableType< TestClassWithPODArray >( "TestClassWithPODArray", new TSerializableTypeInstantiator< TestClassWithPODArray >() );

   DerivedTestClass instA;
   TestClass instB;
   TestClassWithPODArray instC;

   CPPUNIT_ASSERT( instA.isA< DerivedTestClass >() );
   CPPUNIT_ASSERT( instA.isA< TestClass >() );
   CPPUNIT_ASSERT( !instA.isA< TestClassWithPODArray >() );

   CPPUNIT_ASSERT( !instB.isA< DerivedTestClass >() );
   CPPUNIT_ASSERT( instB.isA< TestClass >() );
   CPPUNIT_ASSERT( !instB.isA< TestClassWithPODArray >() );

   CPPUNIT_ASSERT( !instC.isA< DerivedTestClass >() );
   CPPUNIT_ASSERT( !instC.isA< TestClass >() );
   CPPUNIT_ASSERT( instC.isA< TestClassWithPODArray >() );

   CPPUNIT_ASSERT( instA.isA( SerializableReflectionType( "DerivedTestClass" ) ) );
   CPPUNIT_ASSERT( instA.isA( SerializableReflectionType( "TestClass" ) ) );
   CPPUNIT_ASSERT( !instA.isA( SerializableReflectionType( "TestClassWithPODArray" ) ) );

   CPPUNIT_ASSERT( !instB.isA( SerializableReflectionType( "DerivedTestClass" ) ) );
   CPPUNIT_ASSERT( instB.isA( SerializableReflectionType( "TestClass" ) ) );
   CPPUNIT_ASSERT( !instB.isA( SerializableReflectionType( "TestClassWithPODArray" ) ) );

   CPPUNIT_ASSERT( !instC.isA( SerializableReflectionType( "DerivedTestClass" ) ) );
   CPPUNIT_ASSERT( !instC.isA( SerializableReflectionType( "TestClass" ) ) );
   CPPUNIT_ASSERT( instC.isA( SerializableReflectionType( "TestClassWithPODArray" ) ) );

   // cleanup - and now we don't need to delete the restored objects ourselves - the tracker will take care of that
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, loadSaveNotifications )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClassNotifiable >( "TestClassNotifiable", new TSerializableTypeInstantiator< TestClassNotifiable >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the object
   TestClassNotifiable obj( 5 ); 

   // check if the pre-notification managed to populate the other value

   CPPUNIT_ASSERT_EQUAL( 5, obj.m_valOrig );
   CPPUNIT_ASSERT_EQUAL( 0, obj.m_valPreSave );
   CPPUNIT_ASSERT_EQUAL( 0, obj.m_valPostLoad );

   saver.save( obj );

   CPPUNIT_ASSERT_EQUAL( 5, obj.m_valOrig );
   CPPUNIT_ASSERT_EQUAL( 1, obj.m_valPreSave );
   CPPUNIT_ASSERT_EQUAL( 0, obj.m_valPostLoad );

   saver.flush();

   // restore the object
   TestClassNotifiable* restoredObject = loader.load< TestClassNotifiable >();
   CPPUNIT_ASSERT( restoredObject != NULL );
   
   CPPUNIT_ASSERT_EQUAL( 5, obj.m_valOrig );
   CPPUNIT_ASSERT_EQUAL( 1, obj.m_valPreSave );
   CPPUNIT_ASSERT_EQUAL( 0, obj.m_valPostLoad );

   CPPUNIT_ASSERT_EQUAL( 5, restoredObject->m_valOrig );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObject->m_valPreSave );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObject->m_valPostLoad );


   // cleanup
   delete restoredObject;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Reflection, primitiveTypes )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addExternalType< int >( "int" );
   typesRegistry.addExternalType< bool >( "bool" );
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );

   ReflectionType* intType = typesRegistry.find< int >();
   CPPUNIT_ASSERT( NULL != intType );

   ReflectionType* boolType = typesRegistry.find< bool >();
   CPPUNIT_ASSERT( NULL != boolType );

   ReflectionType* testClassType = typesRegistry.find< TestClass >();
   CPPUNIT_ASSERT( NULL != testClassType );

   // this one wasn't defined
   ReflectionType* testNotifiableClassType = typesRegistry.find< TestClassNotifiable >();
   CPPUNIT_ASSERT( NULL == testNotifiableClassType );

   // and neither was this one
   ReflectionType* uintType = typesRegistry.find< uint >();
   CPPUNIT_ASSERT( NULL == uintType );

   CPPUNIT_ASSERT( intType->isExactlyA( *boolType ) == false );
   CPPUNIT_ASSERT( intType->isA( *boolType ) == false );

   CPPUNIT_ASSERT( boolType->isExactlyA( *boolType ) == true );
   CPPUNIT_ASSERT( boolType->isA( *boolType ) == true );

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Reflection, casts )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< TestClass >( "TestClass", new TSerializableTypeInstantiator< TestClass >() );
   typesRegistry.addSerializableType< DerivedTestClass >( "DerivedTestClass", new TSerializableTypeInstantiator< DerivedTestClass >() );

   DerivedTestClass derrivedTestClassObj;

   TestClass* testClassPtr = DynamicCast< TestClass >( &derrivedTestClassObj );
   CPPUNIT_ASSERT( NULL != testClassPtr );

   DerivedTestClass* derrivedTestClassPtr = DynamicCast< DerivedTestClass >( &derrivedTestClassObj );
   CPPUNIT_ASSERT( NULL != derrivedTestClassPtr );

   // cleanup
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////
