#include "ml-Blender/MeshInstantiator.h"
#include "core-Renderer.h"
#include "core.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>


///////////////////////////////////////////////////////////////////////////////

MeshInstantiator::MeshInstantiator( TiXmlNode* meshNode, ResourcesManager& rm )
{
   if ( !meshNode )
   {
      throw std::invalid_argument( "NULL pointer instead a valid TiXmlNode mesh node specified" );
   }

   TiXmlElement* meshElem = meshNode->ToElement();
   ASSERT( meshElem != NULL );

   // get the mesh name
   std::string name = meshElem->Attribute( "name" );

   // get the mesh vertices
   std::vector<LitVertex> vertices;
   TiXmlNode* verticesNode = meshNode->FirstChild( "Vertices" );
   if ( verticesNode )
   {
      for ( TiXmlElement* vtxElem = verticesNode->FirstChildElement( "Vtx" ); vtxElem; vtxElem = vtxElem->NextSiblingElement( "Vtx" ) )
      {    
         vertices.push_back( LitVertex() );
         LitVertex& vtx = vertices.back();

         // parse vertex position
         std::string vtxStr = vtxElem->Attribute( "pos" );
         sscanf_s( vtxStr.c_str(), "%f %f %f", &vtx.m_coords.x, &vtx.m_coords.y, &vtx.m_coords.z );

         // parse vertex normal
         std::string normStr = vtxElem->Attribute( "norm" );
         sscanf_s( normStr.c_str(), "%f %f %f", &vtx.m_normal.x, &vtx.m_normal.y, &vtx.m_normal.z );
      }
   }

   // get the mesh faces
   std::vector<Face> faces;
   TiXmlNode* facesNode = meshNode->FirstChild( "Faces" );
   if ( facesNode )
   {
      for ( TiXmlElement* faceElem = facesNode->FirstChildElement( "Face" ); faceElem; faceElem = faceElem->NextSiblingElement( "Face" ) )
      {    
         // parse the vertex indices
         std::string faceStr = faceElem->Attribute( "idx" );
         unsigned int i1, i2, i3;
         sscanf_s( faceStr.c_str(), "%d %d %d", &i1, &i2, &i3 );

         faces.push_back( Face( i1, i2, i3 ) );
      }
   }

   if ( name.empty() || vertices.empty() || faces.empty() )
   {
      throw std::runtime_error( "Insufficient data to create a mesh" );
   }
   m_mesh = new TriangleMesh( name, vertices, faces );
   
   // register a mesh with the resources manager
   rm.addResource( m_mesh );
}

///////////////////////////////////////////////////////////////////////////////

Entity* MeshInstantiator::instantiate() const
{
   return new Geometry( *m_mesh );
}

///////////////////////////////////////////////////////////////////////////////
