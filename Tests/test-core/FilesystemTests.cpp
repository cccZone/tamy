#include "core-TestFramework\TestFramework.h"
#include "core\Filesystem.h"
#include "core\StreamBuffer.h"


///////////////////////////////////////////////////////////////////////////////

TEST(Filesystem, loadingFilesUnderSpecificRoot)
{
   Filesystem filesystem;

   filesystem.changeRootDir("../Data/");
   CPPUNIT_ASSERT_EQUAL(true, filesystem.doesExist("ala.txt"));
   CPPUNIT_ASSERT_EQUAL(false, filesystem.doesExist("ola.txt"));
   CPPUNIT_ASSERT_EQUAL(true, filesystem.doesExist("family\\ola.txt"));
   CPPUNIT_ASSERT_EQUAL(true, filesystem.doesExist("family/ola.txt"));
   CPPUNIT_ASSERT_EQUAL(false, filesystem.doesExist("family/ala.txt"));

   // same thing will work if we don't specify the closing backslash
   filesystem.changeRootDir("../Data");
   CPPUNIT_ASSERT_EQUAL(true, filesystem.doesExist("ala.txt"));
   CPPUNIT_ASSERT_EQUAL(false, filesystem.doesExist("ola.txt"));
   CPPUNIT_ASSERT_EQUAL(true, filesystem.doesExist("family\\ola.txt"));
   CPPUNIT_ASSERT_EQUAL(true, filesystem.doesExist("family/ola.txt"));
   CPPUNIT_ASSERT_EQUAL(false, filesystem.doesExist("family/ala.txt"));
}

///////////////////////////////////////////////////////////////////////////////

TEST(Filesystem, accessingFileLikeAMemoryBuffer)
{
   Filesystem filesystem;
   filesystem.changeRootDir("../Data/");

   StreamBuffer<char> charBuf(filesystem.open("ala.txt"));
   CPPUNIT_ASSERT_EQUAL((std::size_t)5, charBuf.size());
   CPPUNIT_ASSERT_EQUAL('1', charBuf[0]);
   CPPUNIT_ASSERT_EQUAL('2', charBuf[1]);
   CPPUNIT_ASSERT_EQUAL('3', charBuf[2]);
   CPPUNIT_ASSERT_EQUAL('4', charBuf[3]);
   CPPUNIT_ASSERT_EQUAL('5', charBuf[4]);

   StreamBuffer<int> intTextBuf(filesystem.open("ala.txt"));
   CPPUNIT_ASSERT_EQUAL((std::size_t)2, intTextBuf.size());
   CPPUNIT_ASSERT_EQUAL(875770417, intTextBuf[0]);
   CPPUNIT_ASSERT_EQUAL(53, intTextBuf[1]);
}

///////////////////////////////////////////////////////////////////////////////
