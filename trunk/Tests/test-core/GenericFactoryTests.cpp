#include "core-TestFramework\TestFramework.h"
#include "core\GenericFactory.h"
#include <map>
#include <string>
#include <typeinfo>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class Object
   {
   public:
      virtual ~Object() {}
   };

   // -------------------------------------------------------------------------

   class ObjectA : public Object
   {
   };

   // -------------------------------------------------------------------------

   class ObjectB : public Object
   {
   };

   // -------------------------------------------------------------------------

   class IRepresentation
   {
   public:
      virtual ~IRepresentation() {}

      virtual std::string getID() const = 0;
   };

   // -------------------------------------------------------------------------

   class RepresentationA : public IRepresentation
   {
   public:
      RepresentationA(ObjectA& obj) {}

      std::string getID() const {return "RepresentationA";}
   };

   // -------------------------------------------------------------------------

   class RepresentationB : public IRepresentation 
   {
   public:
      RepresentationB(ObjectB& obj) {}

      std::string getID() const {return "RepresentationB";}
   };
   
} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(GenericFactory, creatingObjects)
{
   GenericFactory<Object, IRepresentation> factory;
   factory.associate<ObjectA, RepresentationA>()
          .associate<ObjectB, RepresentationB>();


   Object* obA = new ObjectA();
   Object* obB = new ObjectB();

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
