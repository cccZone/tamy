#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\EffectAttribute.h"
#include "core-Renderer\AttributeSorterNode.h"
#include "core-Renderer\LeafAttributeSorter.h"
#include <vector>
#include <string>
#include <algorithm>


///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{

   typedef std::vector<std::string> Log;

   // -------------------------------------------------------------------------

   class AttributedMock : public Attributed
   {
   private:
      std::string m_id;
      Log& m_log;
      Attributed::Attributes m_attribs;

   public:
      AttributedMock(const std::string& id, Log& log)
         : m_id(id), m_log(log)
      {}

      ~AttributedMock()
      {
         unsigned int count = m_attribs.size();
         for (unsigned int i = 0; i < count; ++i)
         {
            delete m_attribs[i];
         }
         m_attribs.clear();
      }

      void add(EffectAttribute* attrib)
      {
         m_attribs.push_back(attrib);
      }

      const Attributes& getAttributes() const
      {
         return m_attribs;
      }

      void render() 
      {
         m_log.push_back(m_id);
      }
   };

   // -------------------------------------------------------------------------

   class MaterialAttribMock : public EffectAttribute
   {
   private:
      std::string m_matName;

   public:
      MaterialAttribMock(const std::string& matName)
         : m_matName(matName)
      {}

      bool operator<(const MaterialAttribMock& rhs) const
      {
         return m_matName < rhs.m_matName;
      }
   };

   // -------------------------------------------------------------------------

   class TransparentAttribMock : public EffectAttribute
   {
   };

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(AttributeSorter, typeSensitiveAttribute)
{
   Log log;

   AttributedMock attribsCorrect("attribsCorrect", log);
   attribsCorrect.add(new MaterialAttribMock("mat"));
   AttributedMock attribsIncorrect("attribsIncorrect", log);
   attribsIncorrect.add(new TransparentAttribMock());
   AttributedMock attribsEmpty("attribsEmpty", log);

   LeafAttributeSorter<MaterialAttribMock> sorter;
   sorter.add(attribsCorrect);
   sorter.add(attribsEmpty);
   sorter.add(attribsIncorrect);
   sorter.render();

   CPPUNIT_ASSERT_EQUAL((unsigned int)1, log.size());

   Log::iterator it = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("attribsCorrect"), *(it++));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AttributeSorter, effectsSorting)
{
   Log log;

   LeafAttributeSorter<MaterialAttribMock> sorter;

   AttributedMock renderable1("renderable1", log);
   renderable1.add(new MaterialAttribMock("Material_1"));

   AttributedMock renderable2("renderable2", log);
   renderable2.add(new MaterialAttribMock("Material_2"));

   AttributedMock renderable3("renderable3", log);
   renderable3.add(new MaterialAttribMock("Material_1"));

   sorter.add(renderable1);
   sorter.add(renderable2);
   sorter.add(renderable3);
   sorter.render();

   CPPUNIT_ASSERT_EQUAL((unsigned int)3, log.size());

   Log::iterator it = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("renderable1"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("renderable3"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("renderable2"), *(it++));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AttributeSorter, attributesTree)
{
   Log log;

   TAttributeSorterNode<TransparentAttribMock> sorter;
   sorter.setPrimarySorter(new LeafAttributeSorter<MaterialAttribMock>());

   AttributedMock renderable1("renderable1", log);
   renderable1.add(new MaterialAttribMock("Material_1"));
   renderable1.add(new TransparentAttribMock());

   AttributedMock renderable2("renderable2", log);
   renderable2.add(new MaterialAttribMock("Material_2"));

   AttributedMock renderable3("renderable3", log);
   renderable3.add(new MaterialAttribMock("Material_3"));

   sorter.add(renderable1);
   sorter.add(renderable2);
   sorter.add(renderable3);
   sorter.render();

   CPPUNIT_ASSERT_EQUAL((unsigned int)3, log.size());

   Log::iterator it = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("renderable2"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("renderable3"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("renderable1"), *(it++));
}

///////////////////////////////////////////////////////////////////////////////

TEST(AttributeSorter, removingElementFromTree)
{
   Log log;

   TAttributeSorterNode<TransparentAttribMock> sorter;
   sorter.setPrimarySorter(new LeafAttributeSorter<MaterialAttribMock>());

   AttributedMock renderable1("renderable1", log);
   renderable1.add(new MaterialAttribMock("Material_1"));
   renderable1.add(new TransparentAttribMock());

   AttributedMock renderable2("renderable2", log);
   renderable2.add(new MaterialAttribMock("Material_2"));

   AttributedMock renderable3("renderable3", log);
   renderable3.add(new MaterialAttribMock("Material_3"));

   sorter.add(renderable1);
   sorter.add(renderable2);
   sorter.add(renderable3);
   
   // case 1
   sorter.remove(renderable2);
   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)2, log.size());

   Log::iterator it = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("renderable3"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("renderable1"), *(it++));
   log.clear();

   // case 2
   sorter.remove(renderable1);
   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, log.size());

   it = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("renderable3"), *(it++));
   log.clear();

   // case 3
   sorter.remove(renderable3);
   sorter.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, log.size());
}

///////////////////////////////////////////////////////////////////////////////
