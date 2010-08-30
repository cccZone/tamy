#include "ml-Blender/MeshCS.h"
#include "ml-Blender/BlenderScene.h"
#include "core-Renderer.h"
#include "core.h"
#include "tinyxml/tinyxml.h"
#include <stdexcept>
#include <sstream>


class DataInput
{
public:
   virtual ~DataInput() {}

   virtual void initialize( LitVertex& vertex, int dataOffset ) const = 0;
};

class DataSource
{
public:
   virtual ~DataSource() {}

   virtual void readData( int dataOffset, void* data ) = 0;
};

class SimpleInput : public DataInput
{
private:
   DataSource&    m_source;
   std::string    m_semantic;
public:
   SimpleInput( DataSource& source, const std::string& semantic )
      : m_source( source )
      , m_semantic( semantic )
   {
   }

   void initialize( LitVertex& vertex, int dataOffset ) const
   {
      if ( m_semantic == "NORMAL" )
      {
         m_source.readData( dataOffset, &(vertex.m_normal) );
      }
      else if ( m_semantic == "POSITION" )
      {
         m_source.readData( dataOffset, &(vertex.m_coords) );
      }
      else if ( m_semantic == "VERTEX" )
      {
         m_source.readData( dataOffset, &vertex );
      }
   }
};

class DSArray : public DataSource
{
private:
   std::vector< D3DXVECTOR3 >      m_elems;

public:
   DSArray( TiXmlElement& elem )
   {
      TiXmlElement* techniqueElem = elem.FirstChildElement( "technique_common" );
      ASSERT( techniqueElem != NULL );

      TiXmlElement* accessorElem = techniqueElem->FirstChildElement( "accessor" );
      ASSERT( techniqueElem != NULL );

      int count = 0;
      accessorElem->Attribute( "count", &count );

      TiXmlElement* floatArrayElem = elem.FirstChildElement( "float_array" );
      ASSERT( floatArrayElem != NULL );
      std::string floatArrayData = floatArrayElem->GetText();
      std::stringstream floatArrayStr( floatArrayData );

      float x, y, z;
      for ( int i = 0; i < count; ++i )
      {
         floatArrayStr >> x >> y >> z;
         m_elems.push_back( D3DXVECTOR3( x, y, z ) );
      }
   }

   void readData( int dataOffset, void* data )
   {
      D3DXVECTOR3& vec = *( static_cast< D3DXVECTOR3* >( data ) );
      vec = m_elems[ dataOffset ];
   }
};

class DSVertex : public DataSource
{
private:
   std::map< std::string, DataSource* >&           m_sources;
   std::vector< DataInput* >                       m_inputs;

public:
   DSVertex( TiXmlElement& elem, std::map< std::string, DataSource* >& sources )
      : m_sources( sources )
   {
      for ( TiXmlElement* inputElem = elem.FirstChildElement( "input" ); inputElem != NULL; inputElem = inputElem->NextSiblingElement( "input" ) ) 
      {
         m_inputs.push_back( NULL );
      }

      for ( TiXmlElement* inputElem = elem.FirstChildElement( "input" ); inputElem != NULL; inputElem = inputElem->NextSiblingElement( "input" ) ) 
      {
         std::string semantic = inputElem->Attribute( "semantic" );
         std::string source = inputElem->Attribute( "source" );
         source = source.substr( 1 );

         int offset = 0;
         inputElem->Attribute( "offset", &offset );
         ASSERT( m_inputs[offset] == NULL );

         if ( semantic == "POSITION" )
         {
            m_inputs[offset] = new SimpleInput( *m_sources[ source ], semantic );
         }
      }
   }

   ~DSVertex()
   {
      unsigned int count = m_inputs.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         delete m_inputs[i];
      }
      m_inputs.clear();
   }

   void readData( int dataOffset, void* data )
   {
      LitVertex& vertex = *( static_cast< LitVertex* >( data ) );

      unsigned int count = m_inputs.size();
      for ( unsigned int i = 0; i < count; ++i )
      {
         m_inputs[i]->initialize( vertex, dataOffset );
      }
   }
};

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

   TiXmlElement* polylistElem = meshElem->FirstChildElement( "polylist" );
   ASSERT( polylistElem != NULL );

   TiXmlElement* vcountElem = polylistElem->FirstChildElement( "vcount" );
   ASSERT( vcountElem != NULL );
   std::string vcountData = vcountElem->GetText();
   std::stringstream vcountStr( vcountData );

   TiXmlElement* polyDataElem = polylistElem->FirstChildElement( "p" );
   ASSERT( polyDataElem != NULL );
   std::string polyData = polyDataElem->GetText();
   std::stringstream polyDataStr( polyData );

   std::map< std::string, DataSource* > sources;
   for ( TiXmlElement* sourceElem = meshElem->FirstChildElement( "source" ); sourceElem != NULL; sourceElem = sourceElem->NextSiblingElement( "source" ) ) 
   {
      std::string sourceId = sourceElem->Attribute( "id" );
      sources.insert( std::make_pair( sourceId, new DSArray( *sourceElem ) ) );
   }

   {
      TiXmlElement* verticesElem = meshElem->FirstChildElement( "vertices" );
      ASSERT( verticesElem != NULL );

      std::string sourceId = verticesElem->Attribute( "id" );
      sources.insert( std::make_pair( sourceId, new DSVertex( *verticesElem, sources ) ) );
   }

   // calculate the number of inputs
   std::vector< DataInput* > inputs;
   for ( TiXmlElement* inputElem = polylistElem->FirstChildElement( "input" ); inputElem != NULL; inputElem = inputElem->NextSiblingElement( "input" ) ) 
   {
      inputs.push_back( NULL );
   }

   for ( TiXmlElement* inputElem = polylistElem->FirstChildElement( "input" ); inputElem != NULL; inputElem = inputElem->NextSiblingElement( "input" ) ) 
   {
      std::string semantic = inputElem->Attribute( "semantic" );
      std::string source = inputElem->Attribute( "source" );
      source = source.substr( 1 );

      int offset = 0;
      inputElem->Attribute( "offset", &offset );
      ASSERT( inputs[offset] == NULL );

      inputs[offset] = new SimpleInput( *sources[ source ], semantic );
   }

   // parse the elements
   int facesCount;
   polylistElem->Attribute( "count", &facesCount );

   int                           indicesCount;
   std::vector< LitVertex >      vertices;
   std::vector< Face >           faces;
   LitVertex*                    pVertex = NULL;
   int                           dataOffset;

   for ( int faceIdx = 0; faceIdx < facesCount; ++faceIdx )
   {
      vcountStr >> indicesCount;
      ASSERT( indicesCount >= 3 );
      ASSERT( indicesCount <= 4 );

      unsigned int startVertexIdx = vertices.size();

      // create vertices describing this face
      int inputsCount = inputs.size();
      for ( int i = 0; i < indicesCount; ++i )
      {
         vertices.push_back( LitVertex() );
         pVertex = &vertices.back();

         for ( int j = 0; j < inputsCount; ++j )
         {
            polyDataStr >> dataOffset;
            inputs[ j ]->initialize( *pVertex, dataOffset );
         }
      }

      if ( indicesCount == 3 )
      {
         // a triangle
         faces.push_back( Face( startVertexIdx, startVertexIdx + 1, startVertexIdx + 2 ) );
      }
      else
      {
         // a quad
         faces.push_back( Face( startVertexIdx, startVertexIdx + 1, startVertexIdx + 2 ) );
         faces.push_back( Face( startVertexIdx, startVertexIdx + 2, startVertexIdx + 3 ) );
      }
   }

   if ( vertices.empty() || faces.empty() )
   {
      throw std::runtime_error( "Insufficient data to create a mesh" );
   }
   m_mesh = new TriangleMesh( geometryName, vertices, faces );
   
   // register a mesh with the resources manager
   rm.addResource( m_mesh );

   // cleanup
   for ( std::map< std::string, DataSource* >::iterator it = sources.begin(); it != sources.end(); ++it )
   {
      delete it->second;
   }
   for ( std::vector< DataInput* >::iterator it = inputs.begin(); it != inputs.end(); ++it )
   {
      delete *it;
   }
}

///////////////////////////////////////////////////////////////////////////////

Entity* MeshCS::instantiate( const BlenderScene& host ) const
{
   return new Geometry( *m_mesh );
}

///////////////////////////////////////////////////////////////////////////////
