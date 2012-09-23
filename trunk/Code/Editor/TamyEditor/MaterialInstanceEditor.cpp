#include "MaterialInstanceEditor.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QToolBox>
#include <QToolBar>
#include "ResourceDropArea.h"
#include "MaterialTexturePanel.h"
#include "ColorFrame.h"
#include "core-Renderer.h"
#include "core-MVC/Model.h"
#include "core-MVC/ModelEntity.h"

// scene widget
#include "TamySceneWidget.h"
#include "DebugEntitiesManager.h"
#include "tamyeditor.h"


///////////////////////////////////////////////////////////////////////////////

MaterialInstanceEditor::MaterialInstanceEditor( MaterialInstance& materialInstance )
   : m_materialInstance( materialInstance )
   , m_previewScene( NULL )
   , m_sceneWidget( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

MaterialInstanceEditor::~MaterialInstanceEditor()
{
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onInitialize()
{
   ResourcesManager& resourceMgr = ResourcesManager::getInstance();
   QString iconsDir = resourceMgr.getFilesystem().getShortcut( "editorIcons" ).c_str();

   // setup the UI
   m_ui.setupUi( this );

   // create the toolbar with editor actions
   {
      QToolBar* toolbar = new QToolBar( m_ui.toolbarFrame );
      m_ui.toolbarFrame->layout()->addWidget( toolbar );

      QAction* actionSave = toolbar->addAction( QIcon( iconsDir + tr( "/saveFile.png" ) ), tr( "Save" ) );
      actionSave->setShortcut( QKeySequence( tr( "Ctrl+S" ) ) );
      connect( actionSave, SIGNAL( triggered() ), this, SLOT( save() ) );
   }

   // create the material preview
   {
      FilePath matPreviewPipeline( "/Editor/MaterialInstanceEditor/Pipeline/renderingPipeline.trp" );
      TamyEditor& mainEditor = TamyEditor::getInstance();
      m_sceneWidget = new TamySceneWidget( m_ui.materialPreviewFrame, 0, matPreviewPipeline, mainEditor.getTimeController() );
      m_ui.materialPreviewFrame->layout()->addWidget( m_sceneWidget );

      // disable all debug rendering features
      m_sceneWidget->getDebugEntitiesManager().disableAll();
      m_sceneWidget->getDebugEntitiesManager().showGrid( false );

      // load the debug scene
      m_previewScene = new Model( FilePath() );
      m_sceneWidget->setScene( *m_previewScene );

      // add the preview scene template setting
      FilePath previewScenePath( "/Editor/MaterialInstanceEditor/Scenes/scene.tsc" );
      Model* previewSceneTemplate = resourceMgr.create< Model >( previewScenePath, true );
      if ( previewSceneTemplate )
      {
         m_previewScene->add( new ModelEntity( *previewSceneTemplate, "sceneTemplate" ) );
      }
      
      // create an object we're gonna display the material on
      FilePath sphereMeshPath( "/Editor/MaterialInstanceEditor/Meshes/sphere_0.ttm" );
      TriangleMesh* sphereMesh = resourceMgr.create< TriangleMesh >( sphereMeshPath, true );
      if ( sphereMesh )
      {
         StaticGeometry* materialSphere = new StaticGeometry( *sphereMesh, "SphereMesh" );

         MaterialEntity* materialEntity = new MaterialEntity( "SphereMaterial" );
         materialEntity->setMaterial( &m_materialInstance );
         materialSphere->add( materialEntity );

         m_previewScene->add( materialSphere );
      }

      // position the camera correctly
      Camera& camera = m_sceneWidget->getCamera();
      Matrix& localMtx = camera.accessLocalMtx();
      localMtx.setRows( 
         Vector( 0.69716400f, 0.0f, 0.71691173f, 0.0f ),
         Vector( -0.35515326f, 0.86866885f, 0.34537038f, 0.0f ), 
         Vector( -0.62275887f, -0.49539331f, 0.60560459f, 0.0f ), 
         Vector( 2.7717988, 1.9607872, -2.4661171, 1.0f ) );
   }

   // create a properties browser for the material properties
   {
      // path to the material resource used by this instance
      QGroupBox* resourceGroupBox = new QGroupBox( "Material resource", m_ui.materialPropertiesFrame );
      m_ui.materialPropertiesFrame->layout()->setAlignment( Qt::AlignTop );
      m_ui.materialPropertiesFrame->layout()->setSpacing( 5 );
      m_ui.materialPropertiesFrame->layout()->addWidget( resourceGroupBox );
      {
         QFormLayout* layout = new QFormLayout( resourceGroupBox );
         layout->setMargin( 0 );
         layout->setSpacing( 0 );
         resourceGroupBox->setLayout( layout );

         Material* materialRenderer = m_materialInstance.getMaterialRenderer();
         FilePath materialRendererPath;
         if ( materialRenderer )
         {
            materialRendererPath = materialRenderer->getFilePath();
         }
         m_materialRendererPath = new ResourceDropArea( m_ui.materialPropertiesFrame, materialRendererPath );
         layout->addRow( "Mat. renderer:", m_materialRendererPath );

         connect( m_materialRendererPath, SIGNAL( pathChanged( const FilePath& ) ), this, SLOT( onMaterialRendererChanged( const FilePath& ) ) );
      }

      // color editors for particular surface properties
      QGroupBox* surfacePropsGroupBox = new QGroupBox( "Surface properties", m_ui.materialPropertiesFrame );
      m_ui.materialPropertiesFrame->layout()->addWidget( surfacePropsGroupBox );
      {
         QFormLayout* layout = new QFormLayout( surfacePropsGroupBox );
         layout->setMargin( 0 );
         layout->setSpacing( 0 );
         surfacePropsGroupBox->setLayout( layout );

         const SurfaceProperties& surfaceProps = m_materialInstance.getSurfaceProperties();
         
         m_ambientColor = new ColorFrame( m_ui.materialPropertiesFrame, surfaceProps.getAmbientColor() );
         m_diffuseColor = new ColorFrame( m_ui.materialPropertiesFrame, surfaceProps.getDiffuseColor() );
         m_specularColor = new ColorFrame( m_ui.materialPropertiesFrame, surfaceProps.getSpecularColor() );
         m_emissiveColor = new ColorFrame( m_ui.materialPropertiesFrame, surfaceProps.getEmissiveColor() );
         m_specularPower = new QDoubleSpinBox( m_ui.materialPropertiesFrame );
         m_specularPower->setRange( 0.0f, 100.0f );
         m_specularPower->setValue( surfaceProps.getPower() );

         layout->addRow( "Ambient:", m_ambientColor );
         layout->addRow( "Diffuse:", m_diffuseColor );
         layout->addRow( "Emissive:", m_emissiveColor );
         layout->addRow( "Specular:", m_specularColor );
         layout->addRow( "Specular power:", m_specularPower );

         connect( m_ambientColor, SIGNAL( changed( const QColor& ) ), this, SLOT( onAmbientColorChanged( const QColor& ) ) );
         connect( m_diffuseColor, SIGNAL( changed( const QColor& ) ), this, SLOT( onDiffuseColorChanged( const QColor& ) ) );
         connect( m_emissiveColor, SIGNAL( changed( const QColor& ) ), this, SLOT( onEmissiveColorChanged( const QColor& ) ) );
         connect( m_specularColor, SIGNAL( changed( const QColor& ) ), this, SLOT( onSpecularColorChanged( const QColor& ) ) );
         connect( m_specularPower, SIGNAL( valueChanged( double ) ), this, SLOT( onSpecularPowerChanged( double ) ) );
      }
   }

   // display active textures in the toolbox
   {
      // add the required number of tabs
      QToolBox* texturesToolBox = new QToolBox( m_ui.texturesFrame );
      m_ui.texturesFrame->layout()->addWidget( texturesToolBox );

      ReflectionTypesRegistry& typesRegistry = ReflectionTypesRegistry::getInstance();
      TReflectionEnum< MaterialTextures >* textureUsagesEnum = static_cast< TReflectionEnum< MaterialTextures >* >( typesRegistry.find< MaterialTextures >() );

      std::vector< std::string > textureUsagesNames;
      textureUsagesEnum->getEnumerators( textureUsagesNames );

      for ( uint i = 0; i < MT_MAX; ++i )
      {
         MaterialTextures usage = (MaterialTextures)i;
         MaterialTexturePanel* textureFrame = new MaterialTexturePanel( texturesToolBox, usage, m_materialInstance );
         texturesToolBox->addItem( textureFrame, textureUsagesNames[i].c_str() );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onDeinitialize( bool saveProgress )
{
   if ( m_sceneWidget )
   {
      m_sceneWidget->clearScene();
   }

   if ( m_previewScene )
   {
      m_previewScene->removeReference();
   }
   m_previewScene = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::save()
{
   m_materialInstance.saveResource();
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onMaterialRendererChanged( const FilePath& materialRendererPath )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   Material* materialRenderer = resMgr.create< Material >( materialRendererPath, true );
   m_materialInstance.setMaterialRenderer( materialRenderer );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onAmbientColorChanged( const QColor& color )
{
   Color engineColor = m_ambientColor->getEngineColor();
   m_materialInstance.accessSurfaceProperties().setAmbientColor( engineColor );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onDiffuseColorChanged( const QColor& color )
{
   Color engineColor = m_diffuseColor->getEngineColor();
   m_materialInstance.accessSurfaceProperties().setDiffuseColor( engineColor );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onEmissiveColorChanged( const QColor& color )
{
   Color engineColor = m_emissiveColor->getEngineColor();
   m_materialInstance.accessSurfaceProperties().setEmissiveColor( engineColor );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onSpecularColorChanged( const QColor& color )
{
   Color engineColor = m_specularColor->getEngineColor();
   m_materialInstance.accessSurfaceProperties().setSpecularColor( engineColor );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onSpecularPowerChanged( double specularPower )
{
   m_materialInstance.accessSurfaceProperties().setPower( specularPower );
}

///////////////////////////////////////////////////////////////////////////////

void MaterialInstanceEditor::onTextureChanged( MaterialTextures textureUsage, const FilePath& texturePath )
{
   ResourcesManager& resMgr = ResourcesManager::getInstance();

   Texture* texture = resMgr.create< Texture >( texturePath, true );
   m_materialInstance.setTexture( textureUsage, texture );
}

///////////////////////////////////////////////////////////////////////////////
