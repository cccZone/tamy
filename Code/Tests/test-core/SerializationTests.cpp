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

   struct SerializationTestClass : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_val1;
      int      m_val2;

      SerializationTestClass( int val1 = 0, int val2 = 0, const char* id = NULL ) : ReflectionObject( id ), m_val1( val1 ), m_val2( val2 ) {}
      virtual ~SerializationTestClass() {}
   };
   BEGIN_OBJECT( SerializationTestClass );
      PROPERTY( int, m_val1 );
      PROPERTY( int, m_val2 );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct PatchedSerializationTestClass : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_val;

      PatchedSerializationTestClass( int val = 0 ) : m_val( val ) {}
      virtual ~PatchedSerializationTestClass() {}
   };
   BEGIN_OBJECT( PatchedSerializationTestClass );
      PATCH_MEMBER( m_val2, m_val );
      PROPERTY( int, m_val );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct DerivedSerializationTestClass : public SerializationTestClass
   {
      DECLARE_STRUCT()

      int      m_val3;

      DerivedSerializationTestClass( int val1 = 0, int val2 = 0, int val3 = 0 ) : SerializationTestClass( val1, val2 ), m_val3( val3 ) {}
   };
   BEGIN_OBJECT( DerivedSerializationTestClass );
      PARENT( SerializationTestClass );
      PROPERTY( int, m_val3 );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct PatchedDerivedSerializationTestClass : public PatchedSerializationTestClass
   {
      DECLARE_STRUCT()

      int      m_val2;

      PatchedDerivedSerializationTestClass( int val1 = 0, int val2 = 0 ) : PatchedSerializationTestClass( val1 ), m_val2( val2 ) {}
   };
   BEGIN_OBJECT( PatchedDerivedSerializationTestClass );
      PARENT( PatchedSerializationTestClass );
      PATCH_MEMBER( m_val3, m_val2 );
      PROPERTY( int, m_val2 );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct SerializationTestClassWithPointers : public ReflectionObject
   {
      DECLARE_STRUCT()

      int                           m_val;
      SerializationTestClassWithPointers*        m_ptr;

      SerializationTestClassWithPointers( int val = 0, SerializationTestClassWithPointers* ptr = NULL ) : m_val( val ), m_ptr( ptr ) {}
      ~SerializationTestClassWithPointers() { delete m_ptr; m_ptr = NULL; }
   };
   BEGIN_OBJECT( SerializationTestClassWithPointers );
      PROPERTY( int, m_val );
      PROPERTY( SerializationTestClassWithPointers*, m_ptr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct SerializationTestClassWithPODArray : public ReflectionObject
   {
      DECLARE_STRUCT()

      std::vector< int >                  m_arr;
   };
   BEGIN_OBJECT( SerializationTestClassWithPODArray );
      PROPERTY( std::vector< int >, m_arr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct SerializationTestClassWithPtrArray : public ReflectionObject
   {
      DECLARE_STRUCT()

      std::vector< SerializationTestClass* >           m_arr;

      ~SerializationTestClassWithPtrArray()
      {
         uint count = m_arr.size();
         for ( uint i = 0; i < count; ++i )
         {
            delete m_arr[i];
         }
         m_arr.clear();
      }
   };
   BEGIN_OBJECT( SerializationTestClassWithPtrArray );
      PROPERTY( std::vector< SerializationTestClass* >, m_arr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct SerializationTestClassWithSharedPointers : public ReflectionObject
   {
      DECLARE_STRUCT()

      SerializationTestClass*        m_ptr;

      SerializationTestClassWithSharedPointers( const char* id = NULL, SerializationTestClass* ptr = NULL ) : ReflectionObject( id ), m_ptr( ptr ) {}
      ~SerializationTestClassWithSharedPointers() { m_ptr = NULL; }
   };
   BEGIN_OBJECT( SerializationTestClassWithSharedPointers );
      PROPERTY( SerializationTestClassWithSharedPointers*, m_ptr );
   END_OBJECT();

   // -------------------------------------------------------------------------

   struct SerializationTestClassNotifiable : public ReflectionObject
   {
      DECLARE_STRUCT()

      int      m_valOrig;
      int      m_valPreSave;
      int      m_valPostLoad;

      SerializationTestClassNotifiable( int val = 0 ) : m_valOrig( val ), m_valPreSave( 0 ), m_valPostLoad( 0 ) {}
      virtual ~SerializationTestClassNotifiable() {}

      // ----------------------------------------------------------------------
      // ReflectionObject implementation
      // ----------------------------------------------------------------------
      void onObjectPreSave() { m_valPreSave = 1; }
      void onObjectLoaded() { m_valPostLoad = 1; }
   };
   BEGIN_OBJECT( SerializationTestClassNotifiable );
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

DEFINE_TYPE_ID( SerializationTestClass )
DEFINE_TYPE_ID( PatchedSerializationTestClass )
DEFINE_TYPE_ID( SerializationTestClassNotifiable )
DEFINE_TYPE_ID( SerializationTestClassWithSharedPointers )
DEFINE_TYPE_ID( SerializationTestClassWithPtrArray )
DEFINE_TYPE_ID( SerializationTestClassWithPODArray )
DEFINE_TYPE_ID( SerializationTestClassWithPointers )
DEFINE_TYPE_ID( DerivedSerializationTestClass )
DEFINE_TYPE_ID( PatchedDerivedSerializationTestClass )

///////////////////////////////////////////////////////////////////////////////

TEST( Serialization, simpleTypes )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() ); 
   
   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the object
   SerializationTestClass obj( 5, 10 ); 
   saver.save( obj );
   saver.flush();

   // restore the object
   SerializationTestClass* restoredObject = loader.load< SerializationTestClass >();
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
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   SerializationTestClass obj( 6, 11 ); 
   saver.save( obj );
   saver.flush();

   // clear the types registry
   typesRegistry.clear();

   // register the changed, "patched" type
   typesRegistry.addSerializableType< PatchedSerializationTestClass >( "PatchedSerializationTestClass", new TSerializableTypeInstantiator< PatchedSerializationTestClass >(), "SerializationTestClass" );

   // restore the object
   PatchedSerializationTestClass* restoredObject = loader.load< PatchedSerializationTestClass >();
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
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() );
   typesRegistry.addSerializableType< DerivedSerializationTestClass >( "DerivedSerializationTestClass", new TSerializableTypeInstantiator< DerivedSerializationTestClass >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the object
   DerivedSerializationTestClass obj( 1, 4, 7 ); 
   saver.save( obj );
   saver.flush();

   // restore the object
   SerializationTestClass* restoredObjectBase = loader.load< SerializationTestClass >();
   CPPUNIT_ASSERT( restoredObjectBase != NULL );
   CPPUNIT_ASSERT_EQUAL( 1, restoredObjectBase->m_val1 );
   CPPUNIT_ASSERT_EQUAL( 4, restoredObjectBase->m_val2 );

   DerivedSerializationTestClass* restoredObjectProper = static_cast< DerivedSerializationTestClass* >( restoredObjectBase );
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
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() );
   typesRegistry.addSerializableType< DerivedSerializationTestClass >( "DerivedSerializationTestClass", new TSerializableTypeInstantiator< DerivedSerializationTestClass >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   DerivedSerializationTestClass obj( 3, 2, 8 ); 
   saver.save( obj );
   saver.flush();

   // clear the type definitions and define a new type that has an inheritance definition removed
   typesRegistry.clear();
   typesRegistry.addSerializableType< PatchedSerializationTestClass >( "PatchedSerializationTestClass", new TSerializableTypeInstantiator< PatchedSerializationTestClass >(), "SerializationTestClass" );
   typesRegistry.addSerializableType< PatchedDerivedSerializationTestClass >( "PatchedDerivedSerializationTestClass", new TSerializableTypeInstantiator< PatchedDerivedSerializationTestClass >(), "DerivedSerializationTestClass" );

   // restore the object
   PatchedSerializationTestClass* restoredObjectBase = loader.load< PatchedSerializationTestClass >();
   CPPUNIT_ASSERT( restoredObjectBase != NULL );
   CPPUNIT_ASSERT_EQUAL( 2, restoredObjectBase->m_val );

   PatchedDerivedSerializationTestClass* restoredObjectProper = static_cast< PatchedDerivedSerializationTestClass* >( restoredObjectBase );
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
   typesRegistry.addSerializableType< SerializationTestClassWithPointers >( "SerializationTestClassWithPointers", new TSerializableTypeInstantiator< SerializationTestClassWithPointers >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   SerializationTestClassWithPointers* obj1 = new SerializationTestClassWithPointers( 1 );
   SerializationTestClassWithPointers obj2( 2, obj1 );
   saver.save( obj2 );
   saver.flush();

   // restore the object
   SerializationTestClassWithPointers* restoredObject = loader.load< SerializationTestClassWithPointers >();
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
   typesRegistry.addSerializableType< SerializationTestClassWithPODArray >( "SerializationTestClassWithPODArray", new TSerializableTypeInstantiator< SerializationTestClassWithPODArray >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   SerializationTestClassWithPODArray obj;
   obj.m_arr.push_back( 1 );
   obj.m_arr.push_back( 2 );
   obj.m_arr.push_back( 3 );
   saver.save( obj );
   saver.flush();

   // restore the object
   SerializationTestClassWithPODArray* restoredObject = loader.load< SerializationTestClassWithPODArray >();
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
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() );
   typesRegistry.addSerializableType< SerializationTestClassWithPtrArray >( "SerializationTestClassWithPtrArray", new TSerializableTypeInstantiator< SerializationTestClassWithPtrArray >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   SerializationTestClassWithPtrArray obj;
   obj.m_arr.push_back( new SerializationTestClass( 1, 2 ) );
   obj.m_arr.push_back( new SerializationTestClass( 3, 4 ) );
   obj.m_arr.push_back( new SerializationTestClass( 5, 6 ) );
   saver.save( obj );
   saver.flush();

   // restore the object
   SerializationTestClassWithPtrArray* restoredObject = loader.load< SerializationTestClassWithPtrArray >();
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
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() );
   typesRegistry.addSerializableType< SerializationTestClassWithSharedPointers >( "SerializationTestClassWithSharedPointers", new TSerializableTypeInstantiator< SerializationTestClassWithSharedPointers >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the original object
   SerializationTestClass* sharedObj = new SerializationTestClass( 1, 2 );
   SerializationTestClassWithSharedPointers obj1( "obj1", sharedObj );
   SerializationTestClassWithSharedPointers obj2( "obj1", sharedObj );
   saver.save( obj1 );
   saver.save( obj2 );
   saver.flush();

   // restore the object
   SerializationTestClassWithSharedPointers* restoredObject1 = loader.load< SerializationTestClassWithSharedPointers >();
   SerializationTestClassWithSharedPointers* restoredObject2 = loader.load< SerializationTestClassWithSharedPointers >();
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
   typesRegistry.addSerializableType< SerializationTestClass >( "SerializationTestClass", new TSerializableTypeInstantiator< SerializationTestClass >() );
   typesRegistry.addSerializableType< SerializationTestClassWithSharedPointers >( "SerializationTestClassWithSharedPointers", new TSerializableTypeInstantiator< SerializationTestClassWithSharedPointers >() );

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
   SerializationTestClass* sharedObj = new SerializationTestClass( 1, 2, "sharedObj" );
   SerializationTestClassWithSharedPointers obj1( "obj1", sharedObj );
   SerializationTestClassWithSharedPointers obj2( "obj2", sharedObj );
   saver.save( obj1 );
   saver.flush();
   saver.save( obj2 );
   saver.flush();

   // restore the object
   SerializationTestClassWithSharedPointers* restoredObject1 = loader.load< SerializationTestClassWithSharedPointers >();
   SerializationTestClassWithSharedPointers* restoredObject2 = loader.load< SerializationTestClassWithSharedPointers >();
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

TEST( Serialization, loadSaveNotifications )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< SerializationTestClassNotifiable >( "SerializationTestClassNotifiable", new TSerializableTypeInstantiator< SerializationTestClassNotifiable >() );

   // prepare the serializers
   Array< byte > buffer;
   OutArrayStream outStream( buffer );
   InArrayStream inStream( buffer );
   ReflectionSaver saver( outStream );
   ReflectionLoader loader( inStream );

   // serialize the object
   SerializationTestClassNotifiable obj( 5 ); 

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
   SerializationTestClassNotifiable* restoredObject = loader.load< SerializationTestClassNotifiable >();
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
