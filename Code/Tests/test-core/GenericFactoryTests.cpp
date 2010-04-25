#include "core-TestFramework\TestFramework.h"
#include "core\GenericFactory.h"
#include <map>
#include <string>
#include <typeinfo>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class TemplateObject : public RTTIObject
   {
      DECLARE_RTTI_CLASS

   public:
      virtual ~TemplateObject() {}

      virtual void f() = 0;
   };
   BEGIN_ABSTRACT_RTTI(TemplateObject)
   END_RTTI

   // -------------------------------------------------------------------------

   class TemplateObjectA : public TemplateObject
   {
      DECLARE_RTTI_CLASS

   public:
      void f() {}
   };
   BEGIN_RTTI(TemplateObjectA)
      PARENT(TemplateObject)
   END_RTTI

   // -------------------------------------------------------------------------

   class TemplateObjectB : public TemplateObject
   {
      DECLARE_RTTI_CLASS

   public:
      void f() {}
   };
   BEGIN_RTTI(TemplateObjectB)
      PARENT(TemplateObject)
   END_RTTI

   // -------------------------------------------------------------------------

   class IRepresentation
   {
   public:
      virtual ~IRepresentation() {}

      virtual std::string getID() const = 0;
   };

   // -------------------------------------------------------------------------

   class CommonRepresentation : public IRepresentation
   {
   public:
      CommonRepresentation(TemplateObject& obj) {}

      std::string getID() const {return "CommonRepresentation";}
   };

   // -------------------------------------------------------------------------

   class RepresentationA : public IRepresentation
   {
   public:
      RepresentationA(TemplateObjectA& obj) {}

      std::string getID() const {return "RepresentationA";}
   };

   // -------------------------------------------------------------------------

   class RepresentationB : public IRepresentation 
   {
   public:
      RepresentationB(TemplateObjectB& obj) {}

      std::string getID() const {return "RepresentationB";}
   };
   
} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GenericFactory, creatingObjects)
{
   GenericFactory<TemplateObject, IRepresentation> factory;
   factory.associate<TemplateObjectA, RepresentationA>()
          .associate<TemplateObjectB, RepresentationB>();


   TemplateObject* obA = new TemplateObjectA();
   TemplateObject* obB = new TemplateObjectB();

   IRepresentation* rep = factory.create(*obA);
   CPPUNIT_ASSERT(NULL != rep);
   CPPUNIT_ASSERT_EQUAL(std::string("RepresentationA"), rep->getID());
   delete rep;

   rep = factory.create(*obB);
   CPPUNIT_ASSERT(NULL != rep);
   CPPUNIT_ASSERT_EQUAL(std::string("RepresentationB"), rep->getID());
   delete rep;


   delete obA;
   delete obB;
}

///////////////////////////////////////////////////////////////////////////////

TEST(GenericFactory, creatingObjectsForAbstractClasses)
{
   GenericFactory<TemplateObject, IRepresentation> factory;
   factory.associate<TemplateObjectA, RepresentationA>()
      .associateAbstract<TemplateObject, CommonRepresentation>();


   TemplateObject* obA = new TemplateObjectA();
   TemplateObject* obB = new TemplateObjectB();

   IRepresentation* rep = factory.create(*obA);
   CPPUNIT_ASSERT(NULL != rep);
   CPPUNIT_ASSERT_EQUAL(std::string("RepresentationA"), rep->getID());
   delete rep;

   rep = factory.create(*obB);
   CPPUNIT_ASSERT(NULL != rep);
   CPPUNIT_ASSERT_EQUAL(std::string("CommonRepresentation"), rep->getID());
   delete rep;


   delete obA;
   delete obB;
}

///////////////////////////////////////////////////////////////////////////////
