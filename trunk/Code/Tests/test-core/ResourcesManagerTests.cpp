#include "core-TestFramework\TestFramework.h"
#include "core\Filesystem.h"
#include "core\File.h"
#include "core\Resource.h"
#include "core\ResourcesManager.h"
#include <string>
#include <map>
#include "core\InArrayStream.h"
#include "core\OutArrayStream.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   struct InitializerMock : public Component< ResourcesManager >
   {
      int objsCount;
      InitializerMock() : objsCount(0) {}
   };

   // -------------------------------------------------------------------------

   class ResourceMock : public Resource
   {
      DECLARE_RESOURCE()

   private:
      int m_val;

   public:
      ResourceMock( const FilePath& resourceName = FilePath(), int val = -1 ) 
         : Resource( resourceName )
         , m_val( val ) 
      {}

      int getValue() const { return m_val; }

      void initialize(InitializerMock& initializer)
      {
         ++initializer.objsCount;
      }

      void onComponentAdded( Component< ResourcesManager >& component ) 
      {
         InitializerMock* initializer = dynamic_cast< InitializerMock* >( &component );
         if ( initializer )
         {
            initialize( *initializer );
         }
      }

      void add( ResourceObject* object )
      {
         addObject( object );
      }
   };
   BEGIN_RESOURCE(ResourceMock, txt, AM_BINARY);
      PROPERTY_EDIT("val", int, m_val);
   END_OBJECT();

   // -------------------------------------------------------------------------

   class ObjMock : public ResourceObject
   {
      DECLARE_CLASS()

   private:
      int m_val;

   public:
      ObjMock( int val = -1 ) : m_val( val ) {}
      ~ObjMock()
      {
         m_val = -1;
      }

      int getValue() const { return m_val; }      
   };
   BEGIN_OBJECT( ObjMock );
      PARENT( ResourceObject );
   END_OBJECT();

   // -------------------------------------------------------------------------

   class ResourceWithPointerMock : public Resource
   {
      DECLARE_RESOURCE()

   public:
      Resource*		 m_referencedRes;

   public:
      ResourceWithPointerMock( const FilePath& resourceName = FilePath() ) 
         : Resource( resourceName )
         , m_referencedRes( NULL )
      {}

   };
   BEGIN_RESOURCE( ResourceWithPointerMock, rwp, AM_BINARY );
      PROPERTY_EDIT( "m_referencedRes", Resource*, m_referencedRes );
   END_OBJECT();

} // anonymous

///////////////////////////////////////////////////////////////////////////////

DEFINE_TYPE_ID( ResourceMock )
DEFINE_TYPE_ID( ObjMock )
DEFINE_TYPE_ID( ResourceWithPointerMock )

///////////////////////////////////////////////////////////////////////////////

TEST(ResourcesManager, basic)
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< Resource >( "Resource", NULL );
   typesRegistry.addSerializableType< ObjMock >( "ObjMock", new TSerializableTypeInstantiator< ObjMock >() );
   typesRegistry.addSerializableType< ResourceObject >( "ResourceObject", NULL );
   typesRegistry.addSerializableType< ResourceMock >( "ResourceMock", new TSerializableTypeInstantiator< ResourceMock >() );
   typesRegistry.addSerializableType< ResourceHandle >( "ResourceHandle", NULL  );

   ResourcesManager& manager = ResourcesManager::getInstance();
   manager.setFilesystem( new Filesystem( "..\\Data" ) );

   InitializerMock* initializer = new InitializerMock();
   manager.addComponent( initializer );

   ResourceMock* resourceMock = new ResourceMock( FilePath( "resourceMock.txt" ), 5 );
   manager.addResource( resourceMock );
   resourceMock->saveResource();

   manager.reset();
   initializer->objsCount = 0;

   ResourceMock& res1 = dynamic_cast< ResourceMock& >( manager.create( FilePath( "resourceMock.txt" ) ) );
   CPPUNIT_ASSERT_EQUAL(5, res1.getValue());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer->objsCount);

   ResourceMock& res2 = dynamic_cast< ResourceMock& >( manager.create( FilePath( "resourceMock.txt" ) ) );
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer->objsCount);
   CPPUNIT_ASSERT(&res1 == &res2);

   // clear the types registry
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Resource, resourceHandles )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< Resource >( "Resource", NULL );
   typesRegistry.addSerializableType< ObjMock >( "ObjMock", new TSerializableTypeInstantiator< ObjMock >() );
   typesRegistry.addSerializableType< ResourceObject >( "ResourceObject", NULL );
   typesRegistry.addSerializableType< ResourceMock >( "ResourceMock", new TSerializableTypeInstantiator< ResourceMock >() );
   typesRegistry.addSerializableType< ResourceHandle >( "ResourceHandle", NULL  );

   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();

   FilePath resourceName( "namedResourceMock" );
   ResourceMock* resource = new ResourceMock( resourceName );
   mgr.addResource( resource );
   
   ObjMock* obj1 = new ObjMock( 0 );
   ObjMock* obj2 = new ObjMock( 1 );
   resource->add( obj1 );
   resource->add( obj2 );

   TResourceHandle< ObjMock > hObj1( *obj1 );
   TResourceHandle< ObjMock > hObj2( *obj2 );

   CPPUNIT_ASSERT_EQUAL( 0, hObj1.get().getValue() );
   CPPUNIT_ASSERT_EQUAL( 1, hObj2.get().getValue() );

   mgr.reset();

   // clear the types registry
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( Resource, allRelatedResourcesAreSerializedTogether )
{
   // setup reflection types
   ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
   typesRegistry.addSerializableType< ReflectionObject >( "ReflectionObject", NULL );
   typesRegistry.addSerializableType< Resource >( "Resource", NULL );
   typesRegistry.addSerializableType< ResourceWithPointerMock >( "ResourceWithPointerMock", new TSerializableTypeInstantiator< ResourceWithPointerMock >() );

   // prepare the resources
   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();
   mgr.setFilesystem(new Filesystem("..\\Data"));

   FilePath resource1Name( "res1.rwp" );
   ResourceWithPointerMock* res1 = new ResourceWithPointerMock( resource1Name );
   mgr.addResource( res1 );

   FilePath resource2Name( "res2.rwp" );
   ResourceWithPointerMock* res2 = new ResourceWithPointerMock( resource2Name );
   mgr.addResource( res2 );

   // define the dependencies between the resources
   res1->m_referencedRes = res2;
   res2->m_referencedRes = res1;

   // save the one of the resources
   res1->saveResource();

   // reset the resources manager, so that the original resources get deleted
   mgr.reset();
   res1 = NULL;
   res2 = NULL;

   // restore the resources
   ResourceWithPointerMock* restoredRes1 = static_cast< ResourceWithPointerMock* >( &mgr.create( resource1Name ) );
   CPPUNIT_ASSERT( restoredRes1 != NULL );
   
   // but you'll be able to reclaim it from the resources manager
   Resource* restoredResourceGeneric = mgr.findResource( resource2Name );
   CPPUNIT_ASSERT( restoredResourceGeneric != NULL );
   CPPUNIT_ASSERT( restoredResourceGeneric->isExactlyA< ResourceWithPointerMock >() );

   ResourceWithPointerMock* restoredRes2 = static_cast< ResourceWithPointerMock* >( restoredResourceGeneric );
   CPPUNIT_ASSERT( restoredRes1 != restoredRes2 );
   CPPUNIT_ASSERT( restoredRes1->m_referencedRes != NULL );
   CPPUNIT_ASSERT( restoredRes2->m_referencedRes != NULL );
   CPPUNIT_ASSERT( restoredRes1->m_referencedRes == restoredRes2 );
   CPPUNIT_ASSERT( restoredRes2->m_referencedRes == restoredRes1 );

   // cleanup
   mgr.reset();
   restoredRes1 = NULL;
   restoredRes2 = NULL;
   typesRegistry.clear();
}

///////////////////////////////////////////////////////////////////////////////
