#include "core-TestFramework\TestFramework.h"
#include "core\Filesystem.h"
#include "core\File.h"
#include "core\Resource.h"
#include "core\ResourcesManager.h"
#include <string>
#include <map>


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
      DECLARE_RESOURCE(ResourceMock)

   private:
      int m_val;

   public:
      ResourceMock( const std::string& fileName = "", int val = -1 ) 
         : Resource( fileName )
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
   BEGIN_RESOURCE(ResourceMock, Resource, txt, AM_BINARY)
      PROPERTY_EDIT("val", int, m_val)
   END_OBJECT()

   // -------------------------------------------------------------------------

   class ObjMock : public ResourceObject
   {
      DECLARE_CLASS( ObjMock )

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
   BEGIN_OBJECT( ObjMock, ResourceObject )
   END_OBJECT()

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(ResourcesManager, basic)
{
   ResourcesManager& manager = ResourcesManager::getInstance();
   manager.setFilesystem(new Filesystem("..\\Data"));

   InitializerMock* initializer = new InitializerMock();
   manager.addComponent( initializer );

   ResourceMock* resourceMock = new ResourceMock( "resourceMock.txt", 5 );
   manager.addResource( resourceMock );
   resourceMock->saveResource();

   manager.reset();
   initializer->objsCount = 0;

   ResourceMock& res1 = dynamic_cast< ResourceMock& >( manager.create("resourceMock.txt") );
   CPPUNIT_ASSERT_EQUAL(5, res1.getValue());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer->objsCount);

   ResourceMock& res2 = dynamic_cast< ResourceMock& >( manager.create("resourceMock.txt") );
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer->objsCount);
   CPPUNIT_ASSERT(&res1 == &res2);
}

///////////////////////////////////////////////////////////////////////////////

TEST( Resource, resourceHandles )
{
   ResourcesManager& mgr = ResourcesManager::getInstance();
   mgr.reset();

   const char* resourceName = "namedResourceMock";
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
}

///////////////////////////////////////////////////////////////////////////////
