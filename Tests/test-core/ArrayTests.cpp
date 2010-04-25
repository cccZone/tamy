#include "core-TestFramework\TestFramework.h"
#include "core\Array.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Array, addingNewElements)
{
   Array<int> arr;

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());

   arr.push_back(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(0, arr[0]);

   arr.push_back(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(0, arr[0]);
   CPPUNIT_ASSERT_EQUAL(1, arr[1]);

   arr.push_back(2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(0, arr[0]);
   CPPUNIT_ASSERT_EQUAL(1, arr.at(1));
   CPPUNIT_ASSERT_EQUAL(2, arr[2]);

   arr.push_back(5);
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(0, arr[0]);
   CPPUNIT_ASSERT_EQUAL(1, arr.at(1));
   CPPUNIT_ASSERT_EQUAL(2, arr[2]);
   CPPUNIT_ASSERT_EQUAL(5, arr.at(3));
};

///////////////////////////////////////////////////////////////////////////////

TEST(Array, removingElements)
{
   Array<int> arr;

   arr.push_back(0);
   arr.push_back(1);
   arr.push_back(2);
   arr.push_back(3);

   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(0, arr[0]);
   CPPUNIT_ASSERT_EQUAL(1, arr[1]);
   CPPUNIT_ASSERT_EQUAL(2, arr[2]);
   CPPUNIT_ASSERT_EQUAL(3, arr[3]);

   arr.remove(2);
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(0, arr[0]);
   CPPUNIT_ASSERT_EQUAL(1, arr[1]);
   CPPUNIT_ASSERT_EQUAL(3, arr[2]);

   arr.remove(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(1, arr[0]);
   CPPUNIT_ASSERT_EQUAL(3, arr[1]);

   arr.remove(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
   CPPUNIT_ASSERT_EQUAL(1, arr[0]);

   arr.remove(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.containerSize());
};

///////////////////////////////////////////////////////////////////////////////

TEST(Array, allocate)
{
   Array<int> arr;

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.containerSize());

   arr.allocate(5);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)8, arr.containerSize());

   arr.allocate(27);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)32, arr.containerSize());

   // it doesn't work backwards
   arr.allocate(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)32, arr.containerSize());
};

///////////////////////////////////////////////////////////////////////////////

TEST(Array, resize)
{
   Array<int> arr;

   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.containerSize());

   arr.resize(5);
   CPPUNIT_ASSERT_EQUAL((unsigned int)5, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)8, arr.containerSize());

   arr.resize(27);
   CPPUNIT_ASSERT_EQUAL((unsigned int)27, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)32, arr.containerSize());

   CPPUNIT_ASSERT_EQUAL(0, arr.at(22));

   arr.resize(1);
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)32, arr.containerSize());
};

///////////////////////////////////////////////////////////////////////////////

TEST(Array, initialSize)
{
   Array<int> arr1(5);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr1.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)8, arr1.containerSize());

   Array<int> arr2(0);
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr2.size());
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr2.containerSize());
};

///////////////////////////////////////////////////////////////////////////////

TEST(Array, finding)
{
   Array<int> arr;

   arr.push_back(5);
   arr.push_back(2);
   arr.push_back(9);
   arr.push_back(5); // second item with the same value
   arr.push_back(6);

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, arr.find(2));
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, arr.find(6));
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, arr.find(5));
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, arr.find(5, 1));
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, arr.find(9));

   CPPUNIT_ASSERT_EQUAL(EOA, arr.find(47));
};

///////////////////////////////////////////////////////////////////////////////

TEST(Array, copying)
{
   Array<int> arr;
   arr.push_back(5);
   arr.push_back(2);
   arr.push_back(9);
   arr.push_back(6);

   Array<int> copiedArr;
   copiedArr.push_back(4);
   copiedArr.copyFrom(arr);

   CPPUNIT_ASSERT_EQUAL((unsigned int)5, copiedArr.size());
   CPPUNIT_ASSERT_EQUAL(4, copiedArr[0]);
   CPPUNIT_ASSERT_EQUAL(5, copiedArr[1]);
   CPPUNIT_ASSERT_EQUAL(2, copiedArr[2]);
   CPPUNIT_ASSERT_EQUAL(9, copiedArr[3]);
   CPPUNIT_ASSERT_EQUAL(6, copiedArr[4]);
   CPPUNIT_ASSERT_EQUAL((unsigned int)8, copiedArr.containerSize());
};

///////////////////////////////////////////////////////////////////////////////
