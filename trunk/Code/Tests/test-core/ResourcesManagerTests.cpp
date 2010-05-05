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
   struct InitializerMock
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

      void onResourceLoaded(ResourcesManager& mgr) 
      {
         initialize(mgr.getInitializers().shared<InitializerMock>());
      }
   };
   BEGIN_RESOURCE(ResourceMock, txt, AM_BINARY)
      PROPERTY("val", int, m_val)
   END_OBJECT()

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(ResourcesManager, basic)
{
   ResourcesManager manager;
   manager.setFilesystem(new Filesystem("..\\Data"));

   InitializerMock initializer;
   manager.associate(initializer);

   ResourceMock* resourceMock = new ResourceMock( "resourceMock.txt", 5 );
   manager.addResource( resourceMock );
   resourceMock->saveResource();

   manager.reset();
   initializer.objsCount = 0;

   ResourceMock& res1 = dynamic_cast< ResourceMock& >( manager.create("resourceMock.txt") );
   CPPUNIT_ASSERT_EQUAL(5, res1.getValue());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer.objsCount);

   ResourceMock& res2 = dynamic_cast< ResourceMock& >( manager.create("resourceMock.txt") );
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer.objsCount);
   CPPUNIT_ASSERT(&res1 == &res2);
}

///////////////////////////////////////////////////////////////////////////////
