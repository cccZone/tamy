#include "ml-Blender/SceneCS.h"
#include "ml-Blender/BlenderScene.h"
#include "core.h"
#include "core-MVC.h"
#include "core-Renderer.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

SceneCS::SceneCS( TiXmlNode* sceneNode, ResourcesManager& rm )
   : m_rm( rm )
{
   TiXmlElement* sceneElem = sceneNode->ToElement();
   ASSERT( sceneElem != NULL );

   // find the top nodes and start traversing their hierarchy
   for ( TiXmlElement* nodeElem = sceneElem->FirstChildElement( "node" ); nodeElem != NULL; nodeElem = nodeElem->NextSiblingElement( "node" ) )
   {
      NodeDef* entity = createNode( nodeElem );
      m_nodes.push_back( entity );
   }
}

///////////////////////////////////////////////////////////////////////////////

SceneCS::~SceneCS()
{
   unsigned int count = m_nodes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete m_nodes[i];
   }
   m_nodes.clear();
}

///////////////////////////////////////////////////////////////////////////////

SceneCS::NodeDef* SceneCS::createNode( TiXmlElement* nodeElem )
{
   std::list< NodeDef* >   nodes;
   NodeDef* root = new NodeDef( nodeElem );
   nodes.push_back( root );

   while( !nodes.empty() )
   {
      NodeDef* nodeDef = nodes.front();
      nodes.pop_front();

      for ( TiXmlElement* childElem = nodeDef->nodeElem->FirstChildElement( "node" ); childElem != NULL; childElem = childElem->NextSiblingElement( "node" ) )
      {
         NodeDef* child = new NodeDef( childElem );
         nodeDef->children.push_back( child );

         nodes.push_back( child );
      }
   }

   return root;
}

///////////////////////////////////////////////////////////////////////////////

void SceneCS::instantiate( const BlenderScene& host, Model& scene ) const
{
   unsigned int count = m_nodes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      scene.add( m_nodes[i]->instantiate( host, *this ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

SceneCS::NodeDef* SceneCS::findNode( const std::string& id ) const
{
   unsigned int count = m_nodes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      std::list< NodeDef* > nodes;
      nodes.push_back( m_nodes[i] );

      while( !nodes.empty() )
      {
         NodeDef* nodeDef = nodes.front();
         nodes.pop_front();

         if ( id ==  nodeDef->nodeElem->Attribute( "id" ) )
         {
            return nodeDef;
         }

         unsigned int childrenCount = nodeDef->children.size();
         for (unsigned int i = 0; i < childrenCount; ++i )
         {
            nodes.push_back( nodeDef->children[i] );
         }
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SceneCS::NodeDef::NodeDef( TiXmlElement* elem )
   : nodeElem( elem )
{
}

///////////////////////////////////////////////////////////////////////////////

SceneCS::NodeDef::~NodeDef()
{
   unsigned int count = children.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      delete children[i];
   }
   children.clear();
}

///////////////////////////////////////////////////////////////////////////////

SpatialEntity* SceneCS::NodeDef::instantiate( const BlenderScene& scene, const SceneCS& hostSlice ) const
{
   std::string id = nodeElem->Attribute( "id" );
   SpatialEntity* entity = new SpatialEntity( id );

   if ( id == "chair" )
   {
      printf("");
   }

   // transformation
   float a, b, c, d;
   Quaternion rotation;
   for ( TiXmlElement* rotateElem = nodeElem->FirstChildElement( "rotate" ); rotateElem != NULL; rotateElem = rotateElem->NextSiblingElement( "rotate" ) )
   {
      sscanf_s( rotateElem->GetText(), "%f %f %f %f", &a, &b, &c, &d );

      // switch the y & z components - and that yields the necessity to invert the rotation angle
      Quaternion rotQuat;
      rotQuat.setAxisAngle( Vector( a, b, c ), DEG2RAD( -d ) );

      rotation.mul( rotQuat );
   }

   Matrix& localMtx = entity->accessLocalMtx();
   localMtx = Matrix::IDENTITY;
   {
      TiXmlElement* translateElem = nodeElem->FirstChildElement( "translate" );
      ASSERT( translateElem != NULL );
      sscanf_s( translateElem->GetText(), "%f %f %f", &a, &b, &c );

      // switch the y & z components
      localMtx.setTranslation( Vector( a, b, c ) );
   }
   localMtx.setRotation( rotation );

   // geometry
   for ( TiXmlElement* geometryElem = nodeElem->FirstChildElement( "instance_geometry" ); geometryElem != NULL; geometryElem = geometryElem->NextSiblingElement( "instance_geometry" ))
   {
      std::string geometryURI = geometryElem->Attribute( "url" );
      geometryURI = geometryURI.substr( 1 );

      GeometryResource& geometryRes = scene.getResource< GeometryResource >( geometryURI );
      Entity* geometry = new StaticGeometry( geometryRes );
      entity->add( geometry );

      parseMaterial( *geometryElem, scene, geometry );
   }

   // controllers
   for ( TiXmlElement* controllerElem = nodeElem->FirstChildElement( "instance_controller" ); controllerElem != NULL; controllerElem = controllerElem->NextSiblingElement( "instance_controller" ))
   {
      parseSkeletons( *controllerElem, hostSlice, scene, entity );

      std::string controllerURI = controllerElem->Attribute( "url" );
      controllerURI = controllerURI.substr( 1 );
      Entity* controller = scene.getEntity( controllerURI );
      entity->add( controller );

      parseMaterial( *controllerElem, scene, controller );
   }

   // children
   unsigned int count = children.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      NodeDef* child = children[i];
      SpatialEntity* childEntity = child->instantiate( scene, hostSlice );
      entity->add( childEntity );
   }

   return entity;
}

void SceneCS::NodeDef::parseMaterial( TiXmlElement& elem, const BlenderScene& scene, Entity* entity ) const
{
   // material
   TiXmlElement* materialBindElem = elem.FirstChildElement( "bind_material" );
   if ( materialBindElem )
   {
      TiXmlElement* materialTechniqueElem = materialBindElem->FirstChildElement( "technique_common" );
      ASSERT( materialTechniqueElem != NULL );

      TiXmlElement* materialInstanceElem = materialTechniqueElem->FirstChildElement( "instance_material" );
      ASSERT( materialInstanceElem != NULL );

      std::string materialURI = materialInstanceElem->Attribute( "target" );
      materialURI = materialURI.substr( 1 );

      Entity* material = scene.getEntity( materialURI );
      entity->add( material );
   }
}

void SceneCS::NodeDef::parseSkeletons( TiXmlElement& elem, const SceneCS& hostSlice, const BlenderScene& scene, Entity* entity ) const
{
   TiXmlElement* skeletonElem = elem.FirstChildElement( "skeleton" );
   if ( skeletonElem )
   {
      std::string skeletonURI = skeletonElem->GetText();
      skeletonURI = skeletonURI.substr( 1 );
      
      Entity* skeleton = hostSlice.findNode( skeletonURI )->instantiate( scene, hostSlice );
      entity->add( skeleton );
   }
}
