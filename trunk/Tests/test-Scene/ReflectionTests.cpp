#include "core-TestFramework\TestFramework.h"
#include "core-Scene\ClassesRegistry.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class ClassA
   {
   public:
      int value;

      ClassA(int v = 5) : value(v) {}
      virtual ~ClassA() {}
   };

   class ClassB : public ClassA
   {
   public:
      ClassB(int v = 10) : ClassA(v) {}
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(ClassesRegistry, registeringClasses)
{
   ClassesRegistry registry;

   int classAHandle = registry.define<ClassA>();
   int classBHandle = registry.define<ClassB>();

   ClassA* instanceOfA = (ClassA*)(registry.create(classAHandle));
   CPPUNIT_ASSERT(NULL != instanceOfA);
   CPPUNIT_ASSERT_EQUAL(5, instanceOfA->value);

   ClassB* instanceOfB = (ClassB*)(registry.create(classBHandle));
   CPPUNIT_ASSERT(NULL != instanceOfB);
   CPPUNIT_ASSERT_EQUAL(10, instanceOfB->value);

   ClassA* instanceOfBReferencedByA = (ClassA*)(registry.create(classBHandle));
   CPPUNIT_ASSERT(NULL != instanceOfBReferencedByA);
   CPPUNIT_ASSERT_EQUAL(10, instanceOfBReferencedByA->value);

   delete instanceOfA;
   delete instanceOfB;
   delete instanceOfBReferencedByA;
}

///////////////////////////////////////////////////////////////////////////////

TEST(ClassesRegistry, registeringSameClassTwice)
{
   ClassesRegistry registry;

   int classAFirstHandle = registry.define<ClassA>();
   int classASecondHandle = registry.define<ClassA>();

   CPPUNIT_ASSERT_EQUAL(classAFirstHandle, classASecondHandle);
}

///////////////////////////////////////////////////////////////////////////////
