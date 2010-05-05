#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\GeometryResource.h"
#include "core-Renderer\Effect.h"
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////////

namespace   // anonymous
{

   typedef std::vector<std::string> Log;

   // -------------------------------------------------------------------------

   class GeometryMock : public GeometryResource
   {
   private:
      Log* m_log;
      std::string m_id;

   public:
      GeometryMock() : m_log(NULL) {}
      GeometryMock(Log& log, const std::string& id = "geometry") 
         : m_log(&log)
         , m_id(id)
      {}

      void render()
      {
         m_log->push_back(m_id + " rendered");
      }

      BoundingVolume* calculateBoundingVolume() const {return NULL;}

      void onResourceLoaded(ResourcesManager& mgr) {}
   };

   // -------------------------------------------------------------------------

   class EffectMock : public Effect
   {
      DECLARE_CLASS(EffectMock)

   private:
      Log* m_log;
      unsigned int m_passesCount;
      std::string m_id;

   public:
      EffectMock() : m_log(NULL) {}

      EffectMock(Log& log, 
         unsigned int passesCount = 1, 
         const std::string& id = "effect") 
         : m_log(&log)
         , m_passesCount(passesCount)
         , m_id(id)
      {}

      unsigned int getLayerIdx() const {return 0;}

   protected:
      unsigned int beginRendering()
      {
         m_log->push_back(m_id + " rendering begin");
         return m_passesCount;
      }

      void beginPass(unsigned int passIdx)
      {
         char tmpStr[64];
         sprintf_s(tmpStr, 64, "%s pass %d begin", m_id.c_str(), passIdx);
         m_log->push_back(tmpStr);
      }

      void endPass(unsigned int passIdx)
      {
         char tmpStr[64];
         sprintf_s(tmpStr, 64, "%s pass %d end", m_id.c_str(), passIdx);
         m_log->push_back(tmpStr);
      }

      void endRendering() 
      {
         m_log->push_back(m_id + " rendering end");
      }
   };
   BEGIN_OBJECT(EffectMock, Effect)
   END_OBJECT()

}  // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Effect, renderingProcess)
{
   Log log;

   GeometryMock geometryRes(log);
   Geometry geometry(geometryRes);
   EffectMock effect(log);

   effect.render(geometry);

   CPPUNIT_ASSERT_EQUAL((unsigned int)5, log.size());

   Log::iterator it = log.begin();
   CPPUNIT_ASSERT_EQUAL(std::string("effect rendering begin"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("effect pass 0 begin"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("geometry rendered"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("effect pass 0 end"), *(it++));
   CPPUNIT_ASSERT_EQUAL(std::string("effect rendering end"), *(it++));
}

///////////////////////////////////////////////////////////////////////////////
