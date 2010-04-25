#include "core-TestFramework\TestFramework.h"
#include "core\Component.h"
#include "core\ComponentsManager.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class ComponentsManagerMock : public ComponentsManager<ComponentsManagerMock>
   {
   };

   // -------------------------------------------------------------------------

   class ServiceMock
   {
   private:
      int& m_val;

   public:
      ServiceMock(int& val) : m_val(val) {}

      int& get() {return m_val;}
   };

   // -------------------------------------------------------------------------

   class MockComponentThatProvidesService : public Component<ComponentsManagerMock>
   {
   private:
      int m_val;
      ServiceMock m_service;

   public:
      MockComponentThatProvidesService() 
      : m_val(-1)
      , m_service(m_val)
      {
      }

      void initialize(ComponentsManagerMock& mgr) 
      {
         mgr.registerService<ServiceMock>(m_service);
      }

      void setServiceData(int val) {m_val = val;}
      int getServiceData() const {return  m_val;}
   };

   // -------------------------------------------------------------------------

   class MockComponentThatRequiresService : public Component<ComponentsManagerMock>
   {
   private:
      ServiceMock* m_service;

   public:
      MockComponentThatRequiresService() : m_service(NULL) {}

      void initialize(ComponentsManagerMock& mgr)
      {
         m_service = &(mgr.requestService<ServiceMock>());
      }

      int getServiceData() const
      {
         if (m_service != NULL)
         {
            return m_service->get();
         }
         else
         {
            return -1;
         }
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(ComponentsManager, subscribingToServices)
{
   ComponentsManagerMock compMgr;
   
   MockComponentThatProvidesService* comp1 = new MockComponentThatProvidesService();
   MockComponentThatRequiresService* comp2 = new MockComponentThatRequiresService();
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp1));
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp2));

   comp1->setServiceData(2);
   CPPUNIT_ASSERT_EQUAL(2, comp2->getServiceData());
}

///////////////////////////////////////////////////////////////////////////////
