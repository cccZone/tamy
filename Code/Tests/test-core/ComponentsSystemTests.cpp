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
         registerService( mgr );
      }

      void registerService( ComponentsManagerMock& mgr)  
      {
         mgr.registerService<ServiceMock>( *this, m_service);
      }

      void removeService( ComponentsManagerMock& mgr )
      {
         mgr.removeService<ServiceMock>( *this );
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

      void onServiceRegistered( ComponentsManagerMock& mgr ) 
      {
         if ( mgr.hasService< ServiceMock >() )
         {
            m_service = &( mgr.requestService< ServiceMock >() );
         }
         else
         {
            m_service = NULL;
         }
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

   // -------------------------------------------------------------------------

   class MockComponentWithName : public Component< ComponentsManagerMock >
   {
   public:
      MockComponentWithName( const std::string& name ) 
         : Component< ComponentsManagerMock >( name )
      {
      }
   };

   // -------------------------------------------------------------------------

   class CircularDependencyB;

   class CircularDependencyA : public Component< ComponentsManagerMock >
   {
   public:
      void initialize( ComponentsManagerMock& mgr )
      {
         mgr.registerService< CircularDependencyA >( *this, *this );
      }

      void updateService( ComponentsManagerMock& mgr )
      {
         mgr.registerService< CircularDependencyA >( *this, *this );
      }

      void removeService( ComponentsManagerMock& mgr )
      {
         mgr.removeService< CircularDependencyA >( *this );
      }
   };

   // -------------------------------------------------------------------------

   class CircularDependencyB : public Component< ComponentsManagerMock >
   {
   private:
      CircularDependencyA* m_dep;

   public:
      CircularDependencyB() : m_dep( NULL ) {}

      void initialize( ComponentsManagerMock& mgr )
      {
         mgr.registerService< CircularDependencyB >( *this, *this );
      }

      void onServiceRegistered( ComponentsManagerMock& mgr ) 
      {
         if ( !mgr.needsUpdate< CircularDependencyA >( *m_dep ) )
         {
            return;
         }

         if ( mgr.hasService< CircularDependencyA >() )
         {
            m_dep = &mgr.requestService< CircularDependencyA >();
            mgr.registerService< CircularDependencyB >( *this, *this );
         }
         else
         {
            m_dep = NULL;
            mgr.removeService< CircularDependencyB >( *this );
         }
      }

      bool isServiceUp() const
      {
         return m_dep != NULL;
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST( ComponentsManager, subscribingToServices )
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

TEST( ComponentsManager, notificationsAboutServicesRegistered )
{
   ComponentsManagerMock compMgr;

   MockComponentThatProvidesService* comp1 = new MockComponentThatProvidesService();
   MockComponentThatRequiresService* comp2 = new MockComponentThatRequiresService();

   // the only thing here that differs from test 'subscribingToServices' is the
   // fact that we add the components in the reverse order - and it still should work
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp2));
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp1));

   comp1->setServiceData(2);
   CPPUNIT_ASSERT_EQUAL(2, comp2->getServiceData());
}

///////////////////////////////////////////////////////////////////////////////

TEST( ComponentsManager, removingServices )
{
   ComponentsManagerMock compMgr;

   MockComponentThatProvidesService* comp1 = new MockComponentThatProvidesService();
   MockComponentThatRequiresService* comp2 = new MockComponentThatRequiresService();

   // the only thing here that differs from test 'subscribingToServices' is the
   // fact that we add the components in the reverse order - and it still should work
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp2));
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp1));

   comp1->setServiceData(2);
   CPPUNIT_ASSERT_EQUAL(2, comp2->getServiceData());

   comp1->removeService( compMgr );
   comp1->setServiceData(3);
   // the components become out of sync without a service they can communicate through
   CPPUNIT_ASSERT_EQUAL(-1, comp2->getServiceData());
}

///////////////////////////////////////////////////////////////////////////////

TEST( ComponentsManager, overwritingServices )
{
   ComponentsManagerMock compMgr;

   MockComponentThatProvidesService* comp1 = new MockComponentThatProvidesService();
   MockComponentThatRequiresService* comp2 = new MockComponentThatRequiresService();
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp1));
   CPPUNIT_ASSERT_NO_THROW(compMgr.addComponent(comp2));

   comp1->registerService( compMgr );

   comp1->setServiceData(2);
   CPPUNIT_ASSERT_EQUAL(2, comp2->getServiceData());
}

///////////////////////////////////////////////////////////////////////////////

TEST( ComponentsManager, depdenciesCycle )
{
   ComponentsManagerMock compMgr;

   CircularDependencyA* depA = new CircularDependencyA();
   CircularDependencyB* depB = new CircularDependencyB();
   compMgr.addComponent( depA );
   compMgr.addComponent( depB );
   CPPUNIT_ASSERT_EQUAL( true, depB->isServiceUp() );

   depA->updateService( compMgr );
   CPPUNIT_ASSERT_EQUAL( true, depB->isServiceUp() );

   depA->removeService( compMgr );
   CPPUNIT_ASSERT_EQUAL( false, depB->isServiceUp() );
}

///////////////////////////////////////////////////////////////////////////////

TEST( ComponentsManager, queryingComponentsByName )
{
   ComponentsManagerMock compMgr;
   compMgr.addComponent( new MockComponentWithName("compA") );
   compMgr.addComponent( new MockComponentWithName("compA") );
   compMgr.addComponent( new MockComponentWithName("compB") );

   ComponentsManagerMock::ComponentsArr results;

   compMgr.findComponents( "compA", results );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)2, results.size() );
   results.clear();

   compMgr.findComponents( "compB", results );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)1, results.size() );
   results.clear();

   compMgr.findComponents( "compC", results );
   CPPUNIT_ASSERT_EQUAL( (unsigned int)0, results.size() );
   results.clear();
}

///////////////////////////////////////////////////////////////////////////////

TEST( ComponentsManager, queryingComponentsByType )
{
   ComponentsManagerMock compMgr;
   compMgr.addComponent( new MockComponentWithName("compA") );
   compMgr.addComponent( new MockComponentWithName("compB") );
   compMgr.addComponent( new MockComponentThatProvidesService() );

   {
      std::vector< MockComponentWithName* > results;
      compMgr.findComponents< MockComponentWithName >( results );
      CPPUNIT_ASSERT_EQUAL( (unsigned int)2, results.size() );
   }

   {
      std::vector< MockComponentThatProvidesService* > results;
      compMgr.findComponents< MockComponentThatProvidesService >( results );
      CPPUNIT_ASSERT_EQUAL( (unsigned int)1, results.size() );
   }

   {
      std::vector< MockComponentThatRequiresService* > results;
      compMgr.findComponents< MockComponentThatRequiresService >( results );
      CPPUNIT_ASSERT_EQUAL( (unsigned int)0, results.size() );
   }
}

///////////////////////////////////////////////////////////////////////////////
