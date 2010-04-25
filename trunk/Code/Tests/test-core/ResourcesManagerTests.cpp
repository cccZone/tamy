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
   private:
      int m_val;

   public:
      ResourceMock(Filesystem& fs, const std::string& fileName) 
         : m_val(-1) 
      {
         File* file = fs.open(fileName, std::ios_base::in);
         if (file == NULL)
         {
            throw std::invalid_argument("Test file doesn't exist");
         }

         file->read((byte*)(&m_val), sizeof(int));
         delete file;
      }

      int getValue() const { return m_val; }

      void initialize(InitializerMock& initializer)
      {
         ++initializer.objsCount;
      }

      void onLoaded(ResourcesManager& mgr) 
      {
         initialize(mgr.getInitializers().shared<InitializerMock>());
      }
   };

   // -------------------------------------------------------------------------

   class DifferentResourceMock : public Resource
   {
   public:
      DifferentResourceMock(Filesystem& fs, const std::string& fileName) {}

      void onLoaded(ResourcesManager& mgr) {}
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(ResourcesManager, basic)
{
   ResourcesManager manager;
   manager.setFilesystem(new Filesystem("..\\Data"));

   InitializerMock initializer;
   manager.associate(initializer);

   ResourceMock& res1 = manager.create<ResourceMock>("resourceMock.txt");
   CPPUNIT_ASSERT_EQUAL(5, res1.getValue());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer.objsCount);

   ResourceMock& res2 = manager.create<ResourceMock>("resourceMock.txt");
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());
   CPPUNIT_ASSERT_EQUAL(1, initializer.objsCount);
   CPPUNIT_ASSERT(&res1 == &res2);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ResourcesManager, tryingToReloadSameResourceWithADifferentType)
{
   ResourcesManager manager;
   manager.setFilesystem(new Filesystem("..\\Data"));

   InitializerMock initializer;
   manager.associate(initializer);

   ResourceMock& res1 = manager.create<ResourceMock>("resourceMock.txt");
   CPPUNIT_ASSERT_EQUAL(5, res1.getValue());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, manager.getResourcesCount());

   CPPUNIT_ASSERT_NO_THROW(manager.create<DifferentResourceMock>("resourceMock.txt"));
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, manager.getResourcesCount());
}

///////////////////////////////////////////////////////////////////////////////
