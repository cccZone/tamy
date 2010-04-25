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
   int classAHandle = registry.defineSolid<ClassA>("ClassA");
   int classBHandle = registry.defineSolid<ClassB>("ClassB");

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

   int classAFirstHandle = registry.defineSolid<ClassA>("ClassA");
   int classASecondHandle = registry.defineSolid<ClassA>("ClassA");

   CPPUNIT_ASSERT_EQUAL(classAFirstHandle, classASecondHandle);
}

///////////////////////////////////////////////////////////////////////////////

TEST(ClassesRegistry, creatingInstanceByName)
{
   ClassesRegistry registry;
   registry.defineSolid<ClassA>("ClassA");
   registry.defineSolid<ClassB>("ClassB");

   int handle = registry.getHandle("ClassA");
   ClassA* instance = (ClassA*)registry.create(handle);
   CPPUNIT_ASSERT(NULL != instance);
   delete instance;

   handle = registry.getHandle("ClassB");
   instance = (ClassB*)registry.create(handle);
   CPPUNIT_ASSERT(NULL != instance);
   delete instance;
}

///////////////////////////////////////////////////////////////////////////////
