#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingMechanism.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RenderingMechanismMock : public RenderingMechanism
   {
   private:
      std::string m_id;
      std::vector<std::string>& m_seqLog;

   public:
      RenderingMechanismMock(const std::string& id,
                             std::vector<std::string>& seqLog) 
      : m_id(id), m_seqLog(seqLog)
      {}

      ~RenderingMechanismMock()
      {
      }

      void initialize( Renderer& renderer ) {}

      void render() 
      {
         char tmpStr[64];
         sprintf_s(tmpStr, 64, "%s - render", m_id.c_str());
         m_seqLog.push_back(tmpStr);
      }
   };

   // -------------------------------------------------------------------------

   class RendererImplementationMock : public Renderer
   {
   private:
      std::vector<std::string>& m_seqLog;
      bool m_deviceReady;

   public:
      RendererImplementationMock(std::vector<std::string>& seqLog) 
      : m_seqLog(seqLog)
      , m_deviceReady(true)
      {
      }

      void setDeviceReady(bool enable) 
      {
         m_deviceReady = enable;
      }


   protected:
      void resetViewport(unsigned int width, unsigned int height) {m_seqLog.push_back("Renderer - resetViewport");}

      void renderingBegin() {m_seqLog.push_back("Renderer - begin");}

      void renderingEnd() {m_seqLog.push_back("Renderer - end");}

      bool isGraphicsSystemReady() const {return m_deviceReady;}

      void attemptToRecoverGraphicsSystem() 
      {
         m_seqLog.push_back("Renderer - attemptToRecoverGraphicsSystem");
         m_deviceReady = true;
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, renderingOnlyWhenRenderingDeviceIsReady)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);
   renderer.setMechanism(new RenderingMechanismMock("mech0", 
                                                    callSequence));

   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   renderer.setDeviceReady(false);

   callSequence.clear();
   renderer.render();
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, callSequence.size());
}

///////////////////////////////////////////////////////////////////////////////

TEST(Renderer, recoveringTheLostDevice)
{
   std::vector<std::string> callSequence;

   RendererImplementationMock renderer(callSequence);
   renderer.setMechanism(new RenderingMechanismMock("mech0", 
                                                    callSequence));

   // initial rendering
   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   // we loose the device
   renderer.setDeviceReady(false);
   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)0, callSequence.size()); // we don't attempt to recover the device just yet - let's give it a loop

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)1, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - attemptToRecoverGraphicsSystem"), callSequence[0]);

   // the RendererImplementationMock will consider every recovery attempt a success, so the next
   // rendering pass should proceed as if the device was ready

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)4, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - resetViewport"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[2]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[3]);

   callSequence.clear();
   renderer.render(); 
   CPPUNIT_ASSERT_EQUAL((unsigned int)3, callSequence.size());
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - begin"), callSequence[0]);
   CPPUNIT_ASSERT_EQUAL(std::string("mech0 - render"), callSequence[1]);
   CPPUNIT_ASSERT_EQUAL(std::string("Renderer - end"), callSequence[2]);
}

///////////////////////////////////////////////////////////////////////////////
