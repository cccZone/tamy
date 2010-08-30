#include "ml-Blender/SceneCS.h"
#include "ml-Blender/BlenderScene.h"
#include "core.h"
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
   struct NodeStackElem
   {
      TiXmlElement*     parentElem;
      NodeDef*          node;

      NodeStackElem( TiXmlElement* _elem, NodeDef* _node )
         : parentElem( _elem ), node( _node )
      {}
   };

   std::list< NodeStackElem* >   nodes;
   NodeDef* node = parseEntityNode( nodeElem );
   nodes.push_back( new NodeStackElem( nodeElem, node ) );

   while( !nodes.empty() )
   {
      NodeStackElem* nodeDef = nodes.front();
      nodes.pop_front();

      for ( TiXmlElement* nodeElem = nodeDef->parentElem->FirstChildElement( "node" ); nodeElem != NULL; nodeElem = nodeElem->NextSiblingElement( "node" ) )
      {
         NodeDef* child = parseEntityNode( nodeElem );
         nodeDef->node->children.push_back( child );

         nodes.push_back( new NodeStackElem( nodeElem, child ) );
      }

      delete nodeDef;
   }

   return node;
}

///////////////////////////////////////////////////////////////////////////////

SceneCS::NodeDef* SceneCS::parseEntityNode( TiXmlElement* nodeElem )
{
   std::string id = nodeElem->Attribute( "id" );
   NodeDef* node = new NodeDef( id );

   // transformation
   float a, b, c, d;

   D3DXMATRIX translationMtx;
   {
      TiXmlElement* translateElem = nodeElem->FirstChildElement( "translate" );
      ASSERT( translateElem != NULL );
      sscanf_s( translateElem->GetText(), "%f %f %f", &a, &b, &c );

      D3DXMatrixTranslation( &translationMtx, a, b, c );
   }

   D3DXMATRIX rotationMtx;
   D3DXMatrixIdentity( &rotationMtx );
   for ( TiXmlElement* rotateElem = nodeElem->FirstChildElement( "rotate" ); rotateElem != NULL; rotateElem = rotateElem->NextSiblingElement( "rotate" ) )
   {
      sscanf_s( rotateElem->GetText(), "%f %f %f %f", &a, &b, &c, &d );

      D3DXMATRIX axisRotMtx;
      D3DXMatrixIdentity( &axisRotMtx );
      D3DXMatrixRotationAxis( &axisRotMtx, &D3DXVECTOR3( a, b, c ), DEG2RAD( d ) );
      D3DXMatrixMultiply( &rotationMtx, &axisRotMtx, &rotationMtx );
   }

   D3DXMatrixMultiply( &node->localMtx, &translationMtx, &rotationMtx );

   // geometry
   for ( TiXmlElement* geometryElem = nodeElem->FirstChildElement( "instance_geometry" ); geometryElem != NULL; geometryElem = geometryElem->NextSiblingElement( "instance_geometry" ))
   {
      std::string geometryURI = geometryElem->Attribute( "url" );
      node->geometryURI.push_back( geometryURI.substr( 1 ) );

      // material
      TiXmlElement* materialBindElem = geometryElem->FirstChildElement( "bind_material" );
      if ( !materialBindElem )
      {
         node->materialURI.push_back( "" );
      }
      else
      {
         TiXmlElement* materialTechniqueElem = materialBindElem->FirstChildElement( "technique_common" );
         ASSERT( materialTechniqueElem != NULL );

         TiXmlElement* materialInstanceElem = materialTechniqueElem->FirstChildElement( "instance_material" );
         ASSERT( materialInstanceElem != NULL );

         std::string materialURI = materialInstanceElem->Attribute( "target" );
         node->materialURI.push_back( materialURI.substr( 1 ) );
      }
   }

   return node;
}

///////////////////////////////////////////////////////////////////////////////

void SceneCS::instantiate( const BlenderScene& host, Model& scene ) const
{
   unsigned int count = m_nodes.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      scene.add( m_nodes[i]->instantiate( host ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

SceneCS::NodeDef::NodeDef( const std::string& _id )
   : id( _id )
{
   D3DXMatrixIdentity( &localMtx );
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

SpatialEntity* SceneCS::NodeDef::instantiate( const BlenderScene& scene ) const
{
   SpatialEntity* entity = NULL;
   if ( geometryURI.empty() || materialURI.empty() )
   {
      entity = new RenderableJoint( id );
   }
   else
   {
      entity = new Renderable( id );
   }

   // matrix
   entity->setLocalMtx( localMtx );

   // geometry
   unsigned int count = geometryURI.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      Entity* geometry = scene.getEntity( geometryURI[i] );
      entity->add( geometry );
   }

   // material
   count = materialURI.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      if ( materialURI.empty() )
      {
         continue;
      }

      Entity* material = scene.getEntity( materialURI[i] );
      entity->add( material );
   }

   // instantiate the children
   count = children.size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      entity->add( children[i]->instantiate( scene ) );
   }

   return entity;
}

///////////////////////////////////////////////////////////////////////////////
