#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\RenderCommand.h"
#include <vector>
#include <string>


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class RendererMock : public Renderer
   {
   public:
      std::string                  m_seqLog;

   protected:
      void resetViewport( unsigned int width, unsigned int height ) {}

      void renderingBegin() {}

      void renderingEnd() {}

      bool isGraphicsSystemReady() const { return true; }

      void attemptToRecoverGraphicsSystem() {}

      void activateRenderTarget( RenderTarget* renderTarget, uint targetIdx ) {}

      void deactivateRenderTarget( uint targetIdx ) {}

      void cleanRenderTarget( const Color& bgColor ) {}
   };

   // -------------------------------------------------------------------------

   class RenderingCommandMock : public RenderCommand
   {
   public:
      void execute( Renderer& renderer )
      {
         static_cast< RendererMock& >( renderer ).m_seqLog += "GeometryRenderingCommand;";
      }
   };

   // -------------------------------------------------------------------------

   class RenderingMechanismMock : public RenderingMechanism
   {     
   private:
      bool     m_issueCommand;

   public:
      RenderingMechanismMock( bool issueCommand ) : m_issueCommand( issueCommand ) {}

      void initialize( Renderer& renderer ) {}

      void deinitialize( Renderer& renderer ) {}

      void render( Renderer& renderer )
      {
         if ( m_issueCommand )
         {
            new ( renderer() ) RenderingCommandMock();
         }
      }
   };

}

///////////////////////////////////////////////////////////////////////////////

TEST( RenderingViewTests, simpleCommands )
{
   RendererMock renderer;

   renderer.setMechanism( new RenderingMechanismMock( true ) );
   renderer.render();
   CPPUNIT_ASSERT_EQUAL( std::string( "GeometryRenderingCommand;" ), renderer.m_seqLog );

   renderer.m_seqLog.clear();

   renderer.setMechanism( new RenderingMechanismMock( false ) );
   renderer.render();
   CPPUNIT_ASSERT( renderer.m_seqLog.empty() );
}

///////////////////////////////////////////////////////////////////////////////
