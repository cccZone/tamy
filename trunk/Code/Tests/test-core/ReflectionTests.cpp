#include "core-TestFramework\TestFramework.h"
#include "core\ClassesRegistry.h"
#include "core\Class.h"


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
   ClassTemplate& classAHandle = registry.defineClass< ClassA >();
   ClassTemplate& classBHandle = registry.defineClass< ClassB >();

   ClassA* instanceOfA = (ClassA*)(classAHandle.instantiate());
   CPPUNIT_ASSERT(NULL != instanceOfA);
   CPPUNIT_ASSERT_EQUAL(5, instanceOfA->value);

   ClassB* instanceOfB = (ClassB*)(classBHandle.instantiate());
   CPPUNIT_ASSERT(NULL != instanceOfB);
   CPPUNIT_ASSERT_EQUAL(10, instanceOfB->value);

   ClassA* instanceOfBReferencedByA = (ClassA*)(classBHandle.instantiate());
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

   ClassTemplate& classAFirstHandle = registry.defineClass< ClassA >();
   ClassTemplate& classASecondHandle = registry.defineClass< ClassA >();

   CPPUNIT_ASSERT_EQUAL(classAFirstHandle, classASecondHandle);
}

///////////////////////////////////////////////////////////////////////////////
