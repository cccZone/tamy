#include "core-TestFramework\TestFramework.h"
#include "core-Renderer\Renderer.h"
#include "core-Renderer\RenderingView.h"
#include "core-Renderer\RenderingMechanism.h"
#include "core-Renderer\Geometry.h"
#include "core-Renderer\Camera.h"
#include "core-Renderer\RenderCommand.h"
#include "core-Renderer\RenderState.h"
#include "core\AABoundingBox.h"
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

      void activateRenderTarget( RenderTarget* renderTarget ) {}

      void cleanRenderTarget( const Color& bgColor ) {}
   };

   // -------------------------------------------------------------------------

   class RenderingMechanismMock : public RenderingMechanism
   {     
   private:
      Model&                m_model;
      RenderingView*       m_view;

   public:
      RenderingMechanismMock( Model& model ) : m_model( model ), m_view( NULL ) {}

      ~RenderingMechanismMock()
      {
      }

      void initialize( Renderer& renderer ) 
      {
         m_view = new RenderingView( renderer, AABoundingBox( D3DXVECTOR3( -100, -100, -100 ), D3DXVECTOR3( 100, 100, 100 ) ) );
         m_model.attach( *m_view );
      }

      void deinitialize( Renderer& renderer ) 
      {
         m_model.detach( *m_view );
         delete m_view; m_view = NULL;
      }

      void render( Renderer& renderer )
      {
         m_view->render();
      }
   };

   // -------------------------------------------------------------------------

   class RenderingCommandMock : public RenderCommand
   {
   private:
      std::string    m_id;

   public:
      RenderingCommandMock( const std::string& id ) : m_id( id ) {}

      void execute( Renderer& renderer )
      {
         static_cast< RendererMock& >( renderer ).m_seqLog += m_id + ";";
      }
   };

   // -------------------------------------------------------------------------

   class GeometryMock : public Geometry
   {
   private:
      std::string    m_id;

   public:
      GeometryMock( const std::string& id ) : m_id( std::string( "RenderGeometry_" ) + id ) {}

      void render( Renderer& renderer )
      {
         new ( renderer() ) RenderingCommandMock( m_id );
      }

      BoundingVolume* calculateBoundingVolume() const
      {
         return new AABoundingBox( D3DXVECTOR3( -1, -1, -1 ), D3DXVECTOR3( 1, 1, 1 ) );
      }
   };

   // -------------------------------------------------------------------------

   class RenderStateMock : public RenderState
   {
   private:
      std::string    m_preId;
      std::string    m_postId;

   public:
      RenderStateMock( const std::string& id ) 
         : m_preId( std::string( "set RS " ) + id )
         , m_postId( std::string( "reset RS " ) + id )
      {
         setRenderStateId( atoi( id.c_str() ) );
      }

      void onPreRender( Renderer& renderer )
      {
         new ( renderer() ) RenderingCommandMock( m_preId );
      }

      void onPostRender( Renderer& renderer )
      {
         new ( renderer() ) RenderingCommandMock( m_postId );
      }
   };
}

///////////////////////////////////////////////////////////////////////////////

TEST( RenderingView, basics )
{
   RenderStateMock effect1( "1" );
   RenderStateMock effect2( "2" );

   GeometryMock* g1 = new GeometryMock( "1" );
   GeometryMock* g2 = new GeometryMock( "2" );

   g1->addState( effect1 );
   g2->addState( effect2 );
   
   Model model;
   model.add( g1 );
   model.add( g2 );

   // setup the renderer
   RendererMock renderer;
   renderer.setMechanism( new RenderingMechanismMock( model ) );

   // move the camera so that the entire scene is visible
   D3DXMatrixTranslation( &renderer.getActiveCamera().accessLocalMtx(), 0, 0, -10 );

   // render the scene
   renderer.render();
   CPPUNIT_ASSERT_EQUAL( std::string( "set RS 1;RenderGeometry_1;reset RS 1;set RS 2;RenderGeometry_2;reset RS 2;" ), renderer.m_seqLog );
}

///////////////////////////////////////////////////////////////////////////////

TEST( RenderingView, statesBatching )
{
   RenderStateMock effect1( "1" );
   RenderStateMock effect2( "2" );

   GeometryMock* g1 = new GeometryMock( "1" );
   GeometryMock* g2 = new GeometryMock( "2" );
   GeometryMock* g3 = new GeometryMock( "3" );

   g1->addState( effect1 );      // it uses state 1
   g2->addState( effect2 );      // uses a different state - state 2
   g3->addState( effect1 );      // it also uses state 1

   Model model;
   model.add( g1 );
   model.add( g2 );
   model.add( g3 );

   // setup the renderer
   RendererMock renderer;
   renderer.setMechanism( new RenderingMechanismMock( model ) );

   // move the camera so that the entire scene is visible
   D3DXMatrixTranslation( &renderer.getActiveCamera().accessLocalMtx(), 0, 0, -10 );

   // render the scene
   renderer.render();
   CPPUNIT_ASSERT_EQUAL( std::string( "set RS 1;RenderGeometry_3;RenderGeometry_1;reset RS 1;set RS 2;RenderGeometry_2;reset RS 2;" ), renderer.m_seqLog );
}

///////////////////////////////////////////////////////////////////////////////

