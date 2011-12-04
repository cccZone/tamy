#include "ml-Blender/BlenderScene.h"
#include "tinyxml/tinyxml.h"
#include "core.h"
#include "core-MVC.h"
#include "core-Renderer.h"

// resources instantiators
#include "ml-Blender/EffectCS.h"
#include "ml-Blender/SurfacePropertiesCS.h"
#include "ml-Blender/MeshCS.h"
#include "ml-Blender/ControllerCS.h"
#include "ml-Blender/SceneCS.h"


///////////////////////////////////////////////////////////////////////////////

BlenderScene::BlenderScene( const FilePath& path, ResourcesManager& rm, IProgressObserver* observer )
   : TResourceImporter< Model >( path, rm, observer )
   , m_document( NULL )
{
   // define the slices

   m_definitions.push_back( new TSliceDefinition< SurfacePropertiesCS >( "library_effects", "effect" ) );
   m_definitions.push_back( new TSliceDefinition< EffectCS >( "library_materials", "material" ) );

   m_definitions.push_back( new TSliceDefinition< MeshCS >( "library_geometries", "geometry" ) );
   m_definitions.push_back( new TSliceDefinition< ControllerCS >( "library_controllers", "controller" ) );
   m_definitions.push_back( new TSliceDefinition< SceneCS >( "library_visual_scenes", "visual_scene" ) );
}

///////////////////////////////////////////////////////////////////////////////

BlenderScene::~BlenderScene()
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

void BlenderScene::import( Model& scene )
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
      throw std::runtime_error( "Error loading an XML file" );
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

void BlenderScene::addSceneSlice( ISceneSlice* slice )
{
   if ( slice == NULL )
   {
      throw std::invalid_argument( "NULL pointer instead an IColladaSlice instance ");
   }

   m_sceneSlicesMap.push_back( slice );
}

///////////////////////////////////////////////////////////////////////////////

void BlenderScene::addSlice( const std::string& id, IColladaSlice* slice )
{
   if ( slice == NULL )
   {
      throw std::invalid_argument( "NULL pointer instead an IColladaSlice instance ");
   }

   SlicesMap::const_iterator it = m_slicesMap.find( id );
   if ( it != m_slicesMap.end() )
   {
      throw std::runtime_error( "Slice already exists" );
   }

   m_slicesMap.insert( std::make_pair( id, slice ) );
}

///////////////////////////////////////////////////////////////////////////////

Entity* BlenderScene::getEntity( const std::string& id ) const
{
   SlicesMap::const_iterator it = m_slicesMap.find( id );
   if ( it == m_slicesMap.end() )
   {
      throw std::runtime_error( "Slice doesn't exist" );
   }

   const IEntitySlice* entitySlice = dynamic_cast< const IEntitySlice* >( it->second );
   if ( entitySlice == NULL )
   {
      throw std::runtime_error( "Required slice is does not describe an entity" );
   }

   return entitySlice->instantiate( *this );
}

///////////////////////////////////////////////////////////////////////////////
