#include "ml-Blender/MeshCS.h"
#include "ml-Blender/BlenderScene.h"
#include "core-Renderer.h"
#include "core.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>

#include "ml-Blender/ColladaParser.h"
#include "ml-Blender/DSBindStructure.h"
#include "ml-Blender/DSIndexedBindStructure.h"
#include "ml-Blender/DSIndexed.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{

   class DSVertices : public DSBindStructure
   {
   public:
      DSVertices( TiXmlElement& elem, const ColladaParser& sourcesDB )
         : DSBindStructure( elem, sourcesDB )
      {
      }

   protected:
      void readData( int dataOffset, void* data )
      {
         LitVertex& vtx = *( static_cast< LitVertex* >( data ) );
         getSource( "POSITION" ).readData( dataOffset, &vtx.m_coords );
      }
   };

   struct MeshData
   {
      std::vector< Face >        faces;
      std::vector< LitVertex >   vertices;
   };

   class DSPolygonVertices : public DSIndexedBindStructure
   {
   public:
      DSPolygonVertices( TiXmlElement& elem, const ColladaParser& sourcesDB )
         : DSIndexedBindStructure( elem, sourcesDB )
      {
      }

      void readData( int dataOffset, void* data )
      {
         MeshData* faceData = reinterpret_cast< MeshData* >( data );
         std::vector< Face >& faces = faceData->faces;

         unsigned int indicesCount = getIndicesCount( dataOffset );
         if ( indicesCount == 3 )
         {
            // a triangle
            faces.push_back( Face( getIndex( dataOffset, "VERTEX", 0 ), getIndex( dataOffset, "VERTEX", 1 ), getIndex( dataOffset, "VERTEX", 2 ) ) );
         }
         else
         {
            // a quad
            faces.push_back( Face( getIndex( dataOffset, "VERTEX", 0 ), getIndex( dataOffset, "VERTEX", 1 ), getIndex( dataOffset, "VERTEX", 2 ) ) );
            faces.push_back( Face( getIndex( dataOffset, "VERTEX", 0 ), getIndex( dataOffset, "VERTEX", 2 ), getIndex( dataOffset, "VERTEX", 3 ) ) );
         }

         std::vector< LitVertex >& vertices = faceData->vertices;
         
         for ( unsigned int i = 0; i < indicesCount; ++i )
         {
            unsigned int vtxIdx = getIndex( dataOffset, "VERTEX", i );
            unsigned int normalIdx = getIndex( dataOffset, "NORMAL", i );

            getSource( "NORMAL" ).readData( normalIdx, &vertices[vtxIdx].m_normal );
         }
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

MeshCS::MeshCS( TiXmlNode* geometryNode, ResourcesManager& rm )
{
   if ( !geometryNode )
   {
      throw std::invalid_argument( "NULL pointer instead a valid TiXmlNode mesh node specified" );
   }

   TiXmlElement* geometryElem = geometryNode->ToElement();
   ASSERT( geometryElem != NULL );

   std::string geometryName = geometryElem->Attribute( "id" );

   TiXmlElement* meshElem = geometryElem->FirstChildElement( "mesh" );
   ASSERT( meshElem != NULL );

   ColladaParser parser( *meshElem );
   parser.associate< DSVertices >( "vertices" );
   parser.associate< DSPolygonVertices >( "polylist" );
   parser.initialize();

   MeshData meshData;

   DataSource* verticesData = parser.getStructure( "vertices" );
   unsigned int count = verticesData->size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      meshData.vertices.push_back( LitVertex() );
      LitVertex& vtx = meshData.vertices.back();
      verticesData->readData( i, &vtx );
   }

   DataSource* facesData = parser.getStructure( "polylist" );
   count = facesData->size();
   for ( unsigned int i = 0; i < count; ++i )
   {
      facesData->readData( i, &meshData.faces );
   }

   if ( meshData.vertices.empty() || meshData.faces.empty() )
   {
      throw std::runtime_error( "Insufficient data to create a mesh" );
   }
   m_mesh = new TriangleMesh( geometryName, meshData.vertices, meshData.faces );

   // register a mesh with the resources manager
   rm.addResource( m_mesh );
}

///////////////////////////////////////////////////////////////////////////////
