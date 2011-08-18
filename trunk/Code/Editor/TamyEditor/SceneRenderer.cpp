#include "SceneRenderer.h"
#include "TamySceneWidget.h"
#include <QSettings>


///////////////////////////////////////////////////////////////////////////////

SceneRenderer::SceneRenderer( QWidget* parentWidget )
   : m_parentWidget( parentWidget )
   , m_sceneWidget( NULL )
   , m_scene( NULL )
{

}

///////////////////////////////////////////////////////////////////////////////

SceneRenderer::~SceneRenderer()
{
   delete m_sceneWidget;
   m_sceneWidget = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderer::initialize( TamyEditor& mgr )
{
   // add the widget to the main window
   createRenderer( mgr );
   if ( m_sceneWidget )
   {
      mgr.addToMainWidget( m_sceneWidget );
   }

   // register new services
   mgr.registerService< Renderer >( *this, m_sceneWidget->getRenderer() );
   mgr.registerService< Camera >( *this, m_sceneWidget->getCamera() );
   mgr.registerService< KeysStatusManager >( *this, m_sceneWidget->getKeysStatusManager() );
   mgr.registerService< UserInputController >( *this, *m_sceneWidget );
   mgr.registerService< DebugScene >( *this, m_sceneWidget->getDebugScene() );
   mgr.registerService< CompositeRenderingMechanism >( *this, m_sceneWidget->getRenderingMech() );
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderer::onServiceRegistered( TamyEditor& mgr )
{
   bool sceneChanged = false;

   if ( mgr.hasService< Model >()  )
   {
      if ( mgr.needsUpdate< Model >( *m_scene ) )
      {
         m_scene = &mgr.requestService< Model >();
         sceneChanged = true;
      }
   }
   else
   {
      m_scene = NULL;
      sceneChanged = true;
   }

   if ( sceneChanged && m_sceneWidget )
   {
      m_sceneWidget->setScene( *m_scene );

      mgr.registerService< DebugScene >( *this, m_sceneWidget->getDebugScene() );
   }
}

///////////////////////////////////////////////////////////////////////////////

void SceneRenderer::createRenderer( TamyEditor& mgr )
{
   QSettings& settings = mgr.getSettings();

   settings.beginGroup( "MainRenderer" );
   std::string mainRendererPipelineName = settings.value( "pipeline", "" ).toString().toStdString();
   settings.endGroup();

   // create the widget
   if ( m_sceneWidget )
   {
      delete m_sceneWidget;
      m_sceneWidget = NULL;
   }

   TimeController& timeController = mgr.requestService< TimeController >();
   m_sceneWidget = new TamySceneWidget( m_parentWidget, 0, mainRendererPipelineName, timeController );
}

///////////////////////////////////////////////////////////////////////////////
