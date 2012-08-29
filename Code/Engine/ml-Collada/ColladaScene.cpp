#include "ml-Collada/ColladaScene.h"
#include "tinyxml/tinyxml.h"
#include "core.h"
#include "core-MVC.h"
#include "core-Renderer.h"

// resources instantiators
#include "ml-Collada/EffectCS.h"
#include "ml-Collada/SurfacePropertiesCS.h"
#include "ml-Collada/MeshCS.h"
#include "ml-Collada/ControllerCS.h"
#include "ml-Collada/SceneCS.h"


///////////////////////////////////////////////////////////////////////////////

ColladaScene::ColladaScene( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer )
   : TResourceImporter< Model >( path, rm, observer )
   , m_document( NULL )
{
   path.extractDir( m_deploymentDir );

   // define the slices
   m_definitions.push_back( new TSliceDefinition< SurfacePropertiesCS >( "library_effects", "effect" ) );
   m_definitions.push_back( new TSliceDefinition< EffectCS >( "library_materials", "material" ) );

   m_definitions.push_back( new TSliceDefinition< MeshCS >( "library_geometries", "geometry" ) );
   m_definitions.push_back( new TSliceDefinition< ControllerCS >( "library_controllers", "controller" ) );
   m_definitions.push_back( new TSliceDefinition< SceneCS >( "library_visual_scenes", "visual_scene" ) );
}

///////////////////////////////////////////////////////////////////////////////

ColladaScene::~ColladaScene()
{
   for ( SlicesDefinitions::const_iterator it = m_definitions.begin(); it != m_definitions.end(); ++it )
   {
      delete *it;
   }
   m_definitions.clear();

   for ( SlicesMap::const_iterator it = m_slicesMap.begin(); it != m_slicesMap.end(); ++it )
   {
      delete it->second;
   }
   m_slicesMap.clear();

   delete m_document;
   m_document = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ColladaScene::import( Model& scene )
{
   // load the file contents into a string
   const Filesystem& fs = m_rm.getFilesystem();
   File* sceneFile = fs.open( m_loadedFileName );
   StreamBuffer< char > fileReader( *sceneFile );
   std::string sceneContents = fileReader.getBuffer();
   delete sceneFile;

   unsigned int bufSize = sceneContents.size() + 1;
   char* xmlStr = new char[ bufSize ];
   strcpy_s( xmlStr, bufSize, sceneContents.c_str() );
   m_document = new TiXmlDocument( m_loadedFileName.c_str() );
   bool result = m_document->LoadFile( xmlStr, sceneContents.size(), TIXML_DEFAULT_ENCODING );

   if ( !result )
   {
      ASSERT_MSG( false, "Error loading an XML file" );
      return;
   }

   // start parsing the document
   ASSERT( m_slicesMap.empty() );
   ASSERT( m_sceneSlicesMap.empty() );

   TiXmlNode* sceneSlice = m_document->FirstChild( "COLLADA" );
   if ( !sceneSlice )
   {
      return;
   }

   // parse the definitions
   for ( SlicesDefinitions::const_iterator it = m_definitions.begin(); it != m_definitions.end(); ++it )
   {
      SliceDefinition* def = *it;
      def->parse( sceneSlice, *this );
   }

   // instantiate the scene
   m_observer->initialize( "Instantiating scene objects", m_sceneSlicesMap.size() );
   for ( SceneSlicesMap::const_iterator it = m_sceneSlicesMap.begin(); it != m_sceneSlicesMap.end(); ++it )
   {
      ISceneSlice* slice = *it;
      slice->instantiate( *this, scene );
      delete slice;

      m_observer->advance();
   }
   m_sceneSlicesMap.clear();

   // cleanup
   for ( SlicesMap::iterator it = m_slicesMap.begin(); it != m_slicesMap.end(); ++it )
   {
      delete it->second;
   }
   m_slicesMap.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ColladaScene::addSceneSlice( ISceneSlice* slice )
{
   if ( slice == NULL )
   {
      ASSERT_MSG( false, "NULL pointer instead an IColladaSlice instance ");
      return;
   }

   m_sceneSlicesMap.push_back( slice );
}

///////////////////////////////////////////////////////////////////////////////

void ColladaScene::addSlice( const std::string& id, IColladaSlice* slice )
{
   if ( slice == NULL )
   {
      ASSERT_MSG( false, "NULL pointer instead an IColladaSlice instance ");
      return;
   }

   SlicesMap::const_iterator it = m_slicesMap.find( id );
   if ( it != m_slicesMap.end() )
   {
      ASSERT_MSG( false, "Slice already exists" );
      return;
   }

   m_slicesMap.insert( std::make_pair( id, slice ) );
}

///////////////////////////////////////////////////////////////////////////////

Entity* ColladaScene::getEntity( const std::string& id ) const
{
   SlicesMap::const_iterator it = m_slicesMap.find( id );
   if ( it == m_slicesMap.end() )
   {
      ASSERT_MSG( false, "Slice doesn't exist" );
      return NULL;
   }

   const IEntitySlice* entitySlice = dynamic_cast< const IEntitySlice* >( it->second );
   if ( entitySlice == NULL )
   {
      ASSERT_MSG( false, "Required slice is does not describe an entity" );
      return NULL;
   }

   return entitySlice->instantiate( *this );
}

///////////////////////////////////////////////////////////////////////////////
