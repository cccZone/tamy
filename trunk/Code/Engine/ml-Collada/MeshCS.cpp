#include "ml-Collada/MeshCS.h"
#include "ml-Collada/ColladaScene.h"
#include "core-Renderer.h"
#include "core.h"
#include "tinyxml/tinyxml.h"

#include "ml-Collada/ColladaParser.h"
#include "ml-Collada/DSBindStructure.h"
#include "ml-Collada/DSIndexedBindStructure.h"
#include "ml-Collada/DSIndexed.h"


///////////////////////////////////////////////////////////////////////////////

namespace // anonymous
{
   void adjustCoordinateSystem( TVector< 3 >& vec )
   {
      // switch Y and Z coordinate
      float tmp = vec.v[2];
      vec.v[2] = vec.v[1];
      vec.v[1] = tmp;

      // flip X axis
      vec.v[0] *= -1;
   }

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
         adjustCoordinateSystem( vtx.m_coords );
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
            adjustCoordinateSystem( vertices[vtxIdx].m_normal );
         }
      }
   };

} // namespace anonymous

///////////////////////////////////////////////////////////////////////////////

MeshCS::MeshCS( TiXmlNode* geometryNode, const FilePath& deploymentDir, ResourcesManager& rm )
{
   if ( !geometryNode )
   {
      ASSERT_MSG( false, "NULL pointer instead a valid TiXmlNode mesh node specified" );
      return;
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
      ASSERT_MSG( false, "Insufficient data to create a mesh" );
      return;
   }

   FilePath geometryPath( deploymentDir.getRelativePath() + geometryName + "." + TriangleMesh::getExtension() );
   m_mesh = new TriangleMesh( geometryPath, meshData.vertices, meshData.faces );
   m_mesh->calculateTangents();

   // register a mesh with the resources manager
   rm.addResource( m_mesh );
}

///////////////////////////////////////////////////////////////////////////////
