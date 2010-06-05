#include "SceneNavigator.h"
#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "ext-MotionControllers.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "InputInterpreter.h"
#include "PropertiesEditor.h"
#include "core\Assert.h"

// commands
#include "EditEntityCommand.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   class CameraRotateCommand : public InputCommand
   {
   private:
      SceneNavigator&   m_host;

   public:
      CameraRotateCommand( SceneNavigator& host )
         : m_host( host ) 
      {}

      // ----------------------------------------------------------------------
      // InputCommand implementation
      // ----------------------------------------------------------------------
      void execute( InputState mode, const D3DXVECTOR2& mousePos )
      {
         if ( mode == IS_PRESSED || mode == IS_HELD )
         {
            m_host.setCameraRotation( true );
         }
         else if ( mode == IS_RELEASED )
         {
            m_host.setCameraRotation( false );
         }
      }
   };

   // -------------------------------------------------------------------------

   class CameraMoveCommand : public InputCommand
   {
   private:
      SceneNavigator&                     m_host;
      MovementDirection                   m_direction;

   public:
      CameraMoveCommand( MovementDirection direction, SceneNavigator& host )
         : m_host( host ) 
         , m_direction( direction )
      {}

      // ----------------------------------------------------------------------
      // InputCommand implementation
      // ----------------------------------------------------------------------
      void execute( InputState mode, const D3DXVECTOR2& mousePos )
      {
         if ( mode == IS_PRESSED || mode == IS_HELD )
         {
            m_host.setCameraMove( m_direction, true );
         }
         else if ( mode == IS_RELEASED )
         {
            m_host.setCameraMove( m_direction, false );
         }
      }
   };

} // anonymous

///////////////////////////////////////////////////////////////////////////////

SceneNavigator::SceneNavigator()
: m_camera( NULL )
, m_uic( NULL )
, m_renderer( NULL )
, m_ksm( NULL )
, m_scene( NULL )
, m_selectionMgr( NULL )
, m_mgr( NULL )
, m_cameraController( NULL )
, m_timeController( NULL )
, m_rotating( false )
{
   m_movementDir[0] = m_movementDir[1] = m_movementDir[2] = m_movementDir[3] = false;
}

///////////////////////////////////////////////////////////////////////////////

SceneNavigator::~SceneNavigator()
{
   reset();
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::initialize( TamyEditor& mgr )
{
   ASSERT( !m_mgr, "Component is already managed" );
   m_mgr = &mgr;
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::onServiceRegistered( TamyEditor& mgr )
{
   if ( !mgr.needsUpdate< Camera >( *m_camera )
      || !mgr.needsUpdate< UserInputController >( *m_uic )
      || !mgr.needsUpdate< Renderer >( *m_renderer )
      || !mgr.needsUpdate< TimeController >( *m_timeController )
      || !mgr.needsUpdate< KeysStatusManager >( *m_ksm ) 
      || !mgr.needsUpdate< SceneQueries >( *m_scene )
      || !mgr.needsUpdate< SelectionManager >( *m_selectionMgr ))
   {
      return;
   }

   reset();

   if ( !mgr.hasService< Camera >() 
      || !mgr.hasService< UserInputController >() 
      || !mgr.hasService< Renderer >() 
      || !mgr.hasService< TimeController >() 
      || !mgr.hasService< KeysStatusManager >() 
      || !mgr.hasService< SceneQueries >()
      || !mgr.hasService< SelectionManager >())
   {
      return;
   }

   m_camera = &mgr.requestService<Camera>();
   m_uic = &mgr.requestService<UserInputController> ();
   m_renderer = &mgr.requestService<Renderer> ();
   m_ksm = &mgr.requestService< KeysStatusManager >();
   m_timeController = &mgr.requestService< TimeController >();
   m_scene = &mgr.requestService< SceneQueries >();
   m_selectionMgr = &mgr.requestService< SelectionManager >();

   m_timeController->add( "sceneNavigation" );
   TimeControllerTrack& timeTrack = m_timeController->get( "sceneNavigation" );
   timeTrack.add( new TTimeDependent< SceneNavigator >( *this ) );

   m_cameraController = new UnconstrainedMotionController( *m_camera );
   InputInterpreter* inputHandler = new InputInterpreter( *m_uic, 
                                                          *m_renderer );
   m_ksm->addHandler( inputHandler );

   // configure input commands
   inputHandler->addCommand( VK_LBUTTON, new EditEntityCommand( *m_scene, *m_selectionMgr, *m_uic, *m_camera, timeTrack ) );

   inputHandler->addCommand( VK_RBUTTON, new CameraRotateCommand( *this ) );
   inputHandler->addCommand( 'W', new CameraMoveCommand( MD_FRONT, *this ) );
   inputHandler->addCommand( 'S', new CameraMoveCommand( MD_BACK, *this ) );
   inputHandler->addCommand( 'A', new CameraMoveCommand( MD_LEFT, *this ) );
   inputHandler->addCommand( 'D', new CameraMoveCommand( MD_RIGHT, *this ) );
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::reset()
{
   delete m_cameraController; 
   m_cameraController = NULL;

   if ( m_timeController )
   {
      m_timeController->remove("sceneNavigation");
      m_timeController = NULL;
   }

   if ( m_ksm )
   {
      m_ksm->removeAllHandlers();
      m_ksm = NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::update( float timeElapsed )
{
   float movementSpeed = 40 * timeElapsed;
   float rotationSpeed = 0.1f * timeElapsed;

   // process the keys
   if ( m_movementDir[MD_FRONT] )  { m_cameraController->move( m_cameraController->getLookVec()   * movementSpeed ); }
   if ( m_movementDir[MD_BACK] )   { m_cameraController->move( -m_cameraController->getLookVec()  * movementSpeed ); }
   if ( m_movementDir[MD_LEFT] )   { m_cameraController->move( -m_cameraController->getRightVec() * movementSpeed ); }
   if ( m_movementDir[MD_RIGHT] )  { m_cameraController->move( m_cameraController->getRightVec()  * movementSpeed ); }

   if ( m_rotating )
   {
      D3DXVECTOR2 mouseSpeed = m_uic->getMouseSpeed() * rotationSpeed;
      m_cameraController->rotate( mouseSpeed.y, mouseSpeed.x, 0 );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::setCameraRotation( bool engage )
{
   m_rotating = engage;
   m_uic->setRelativeMouseMovement( m_rotating );
}

///////////////////////////////////////////////////////////////////////////////

void SceneNavigator::setCameraMove( MovementDirection direction, bool engage )
{
   m_movementDir[direction] = engage;
}

///////////////////////////////////////////////////////////////////////////////
