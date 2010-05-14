#include "SceneNavigator.h"
#include "tamyeditor.h"
#include "core-AppFlow.h"
#include "ext-MotionControllers.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "CameraController.h"
#include "PropertiesEditor.h"
#include "core\Assert.h"

// commands
#include "SelectEntityCommand.h"

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
{
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
      || !mgr.needsUpdate< QueryableScene >( *m_scene )
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
      || !mgr.hasService< QueryableScene >()
      || !mgr.hasService< SelectionManager >())
   {
      return;
   }

   m_camera = &mgr.requestService<Camera>();
   m_uic = &mgr.requestService<UserInputController> ();
   m_renderer = &mgr.requestService<Renderer> ();
   m_ksm = &mgr.requestService< KeysStatusManager >();
   m_timeController = &mgr.requestService< TimeController >();
   m_scene = &mgr.requestService< QueryableScene >();
   m_selectionMgr = &mgr.requestService< SelectionManager >();

   m_cameraController = new UnconstrainedMotionController( *m_camera );
   CameraController* cameraAnimator = new CameraController( *m_cameraController,
                                                            *m_uic, 
                                                            *m_renderer );

   m_ksm->addHandler( cameraAnimator );
   m_timeController->add( "sceneNavigation" );
   m_timeController->get( "sceneNavigation" ).add( new TTimeDependent< CameraController >( *cameraAnimator ) );

   // configure input commands
   cameraAnimator->addButtonPressCommand( VK_LBUTTON, new SelectEntityCommand( *m_camera, *m_scene, *m_selectionMgr ) );
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

   m_mgr->removeService< CameraController >( *this );
}

///////////////////////////////////////////////////////////////////////////////
