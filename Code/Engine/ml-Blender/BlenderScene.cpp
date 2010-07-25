#include "ml-Blender/BlenderScene.h"
#include "tinyxml/tinyxml.h"
#include "core.h"
#include "core-MVC.h"
#include "core-Renderer.h"

// resources instantiators
#include "ml-Blender/MeshInstantiator.h"
#include "ml-Blender/MaterialInstantiator.h"


///////////////////////////////////////////////////////////////////////////////

BlenderScene::BlenderScene( const Filesystem& fs, const std::string& fileName, ResourcesManager& rm, IProgressObserver& observer )
   : m_rm( rm )
   , m_observer( observer ) 
{
   // load the file contents into a string
   File* sceneFile = fs.open( fileName );
   StreamBuffer< char > fileReader( *sceneFile );
   std::string sceneContents = fileReader.getBuffer();
   delete sceneFile;

   unsigned int bufSize = sceneContents.size() + 1;
   char* xmlStr = new char[ bufSize ];
   strcpy_s( xmlStr, bufSize, sceneContents.c_str() );
   m_document = new TiXmlDocument( fileName.c_str() );
   bool result = m_document->LoadFile( xmlStr, sceneContents.size(), TIXML_DEFAULT_ENCODING );

   if ( !result )
   {
      throw std::runtime_error( "Error loading an XML file" );
   }
}

///////////////////////////////////////////////////////////////////////////////

BlenderScene::~BlenderScene()
{
   releaseResourceInstantiators();

   delete m_document;
   m_document = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void BlenderScene::load( Model& scene )
{
   releaseResourceInstantiators();

   TiXmlNode* sceneSlice = m_document->FirstChild( "Scene" );
   if ( !sceneSlice )
   {
      return;
   }
   parseResources( sceneSlice );

   parseObjects( sceneSlice, scene );
}

///////////////////////////////////////////////////////////////////////////////

void BlenderScene::parseResources( TiXmlNode* sceneSlice )
{
   TiXmlElement* objectsSlice = sceneSlice->FirstChildElement( "Resources" );
   if ( !objectsSlice )
   {
      return;
   }

   // initialize the observer
   unsigned int elemsCount = 0;
   for ( TiXmlNode* res = objectsSlice->FirstChild( "Resource" ); res; res = res->NextSibling( "Resource" ), ++elemsCount ) {}
   m_observer.initialize( "Loading resources", elemsCount );

   // load the resources
   for ( TiXmlNode* resNode = objectsSlice->FirstChild( "Resource" ); resNode; resNode = resNode->NextSibling( "Resource" ) ) 
   {
      TiXmlElement* resElem = resNode->ToElement();
      ASSERT( resElem != NULL );

      IResourceInstantiator* instantiator = NULL;

      // create an instantiator suitable to the resource type
      std::string type = resElem->Attribute( "type" );
      if ( type == "MESH" )
      {
         instantiator = new MeshInstantiator( resNode, m_rm );
      }
      else if ( type == "MATERIAL" )
      {
         instantiator = new MaterialInstantiator( resNode, m_rm );
      }

      // register the instantiator
      m_instantiators.push_back( instantiator );

      m_observer.advance();
   }
}

///////////////////////////////////////////////////////////////////////////////

void BlenderScene::releaseResourceInstantiators()
{
   unsigned int count = m_instantiators.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_instantiators[i];
   }
   m_instantiators.clear();
}

///////////////////////////////////////////////////////////////////////////////

void BlenderScene::parseObjects( TiXmlNode* sceneSlice, Model& scene )
{
   TiXmlElement* objectsSlice = sceneSlice->FirstChildElement( "Objects" );
   if ( !objectsSlice )
   {
      return;
   }

   // initialize the observer
   unsigned int elemsCount = 0;
   for ( TiXmlNode* objNode = objectsSlice->FirstChild( "Object" ); objNode; objNode = objNode->NextSibling( "Object" ), ++elemsCount ) {}
   m_observer.initialize( "Creating objects", elemsCount );

   // create the objects hierarchy
   for ( TiXmlNode* objNode = objectsSlice->FirstChild( "Object" ); objNode; objNode = objNode->NextSibling( "Object" ) )
   {
      ASSERT( objNode != NULL );
      SpatialEntity* renderable = createHierarchicalObject( objNode );
      if ( renderable )
      {
         // add the renderable to the scene
         scene.add( renderable );
      }

      // advance the observer
      m_observer.advance();
   }

}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* BlenderScene::createHierarchicalObject( TiXmlNode* rootNode )
{
   TiXmlElement* elem = rootNode->ToElement();
   if ( !elem )
   {
      return NULL;
   }

   std::string name = elem->Attribute( "name" );

   // if there are any embedded resources in this object, 
   // we're gonna need a Renderable - otherwise we can use
   // a cheaper version - a RenderableJoint
   SpatialEntity* rootRenderable = NULL;
   if ( rootNode->FirstChild( "Resource" ) == NULL )
   {
      rootRenderable = new RenderableJoint( name );
   }
   else
   {
      rootRenderable = new Renderable( name );
   }

   // get the transformation of the object
   TiXmlNode* transformNode = rootNode->FirstChild ("Transform" );
   ASSERT( transformNode != NULL );
   TiXmlElement* transformElem = transformNode->ToElement();
   ASSERT( transformElem != NULL );
   getObjectTransform( transformElem, rootRenderable->accessLocalMtx() );

   // go through the embedded resources and add them
   for ( TiXmlNode* resNode = rootNode->FirstChild( "Resource" ); resNode; resNode = resNode->NextSibling( "Resource" ) )
   {
      ASSERT( resNode != NULL );
      TiXmlElement* resElem = resNode->ToElement();
      ASSERT( resElem != NULL );

      int resourceIdx = -1;
      resElem->Attribute( "id", &resourceIdx );
      ASSERT( resourceIdx >= 0 );

      IResourceInstantiator* instantiator = m_instantiators.at( resourceIdx );
      if ( instantiator ) // an instantiator may not exist - take that into account
      {
         Entity* resEntity = instantiator->instantiate();
         ASSERT( resEntity != NULL );
         rootRenderable->add( resEntity );
      }
   }

   // go through the children and add them
   for ( TiXmlNode* objNode = rootNode->FirstChild( "Object" ); objNode; objNode = objNode->NextSibling( "Object" ) )
   {
      SpatialEntity* child = createHierarchicalObject( objNode );
      if ( child )
      {
         rootRenderable->add( child );
      }
   }

   return rootRenderable;
}

///////////////////////////////////////////////////////////////////////////////

void BlenderScene::getObjectTransform( TiXmlElement* transformElem, D3DXMATRIX& outTransform ) const
{
   ASSERT( transformElem != NULL );

   std::string translationStr = transformElem->Attribute( "translation" );
   std::string orientationStr = transformElem->Attribute( "rotation" );

   D3DXVECTOR3 objectPos;
   sscanf_s( translationStr.c_str(), "%f %f %f", &objectPos.x, &objectPos.y, &objectPos.z );

   D3DXVECTOR3 rotAxis;
   float rotAngle;
   sscanf_s( orientationStr.c_str(), "%f %f %f %f", &rotAxis.x, &rotAxis.y, &rotAxis.z, &rotAngle );

   D3DXMATRIX mtxTranslation;
   D3DXMatrixTranslation( &mtxTranslation, objectPos.x, objectPos.y, objectPos.z );
   D3DXMatrixRotationAxis( &outTransform, &rotAxis, rotAngle );
   D3DXMatrixMultiply( &outTransform, &outTransform, &mtxTranslation );
}

///////////////////////////////////////////////////////////////////////////////
