#include "ml-Blender/DSIndexedBindStructure.h"
#include "ml-Blender/ColladaParser.h"
#include "tinyxml/tinyxml.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

DSIndexedBindStructure::DSIndexedBindStructure( TiXmlElement& elem, const ColladaParser& sourcesDB )
   : m_sourcesDB( sourcesDB )
{
   // parse the structure
   for ( TiXmlElement* inputElem = elem.FirstChildElement( "input" ); inputElem != NULL; inputElem = inputElem->NextSiblingElement( "input" ) ) 
   {
      std::string semantic = inputElem->Attribute( "semantic" );
      std::string source = inputElem->Attribute( "source" );
      source = source.substr( 1 );

      unsigned int offset = 0;
      inputElem->Attribute( "offset", (int*)&offset );

      m_inputs.insert( std::make_pair( semantic, new Input( offset, source ) ) );
   }

   TiXmlElement* polyDataElem = elem.FirstChildElement( "p" );
   if ( !polyDataElem )
   {
      polyDataElem = elem.FirstChildElement( "v" );
   }
   ASSERT( polyDataElem != NULL );
   std::string dataPtrs = polyDataElem->GetText();
   std::stringstream dataPtrsStr( dataPtrs );

   TiXmlElement* vcountElem = elem.FirstChildElement( "vcount" );
   ASSERT( vcountElem != NULL );
   std::string vcountData = vcountElem->GetText();
   std::stringstream vcountStr( vcountData );

   unsigned int entriesCount;
   elem.Attribute( "count", (int*)&entriesCount );

   unsigned int ptrVal, vCount;
   for ( unsigned int i = 0; i < entriesCount; ++i )
   {
      vcountStr >> vCount;

      unsigned int elemsCount = vCount * m_inputs.size();
      m_elems.push_back( std::vector< unsigned int >( elemsCount ) );
      std::vector< unsigned int >& subElems = m_elems.back();

      for ( unsigned int j = 0; j < elemsCount; ++j )
      {
         dataPtrsStr >> ptrVal;
         subElems[j] = ptrVal;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

DSIndexedBindStructure::~DSIndexedBindStructure()
{
   // cleanup
   for ( std::map< std::string, Input* >::iterator it = m_inputs.begin(); it != m_inputs.end(); ++it )
   {
      delete it->second;
   }
   m_inputs.clear();
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DSIndexedBindStructure::size() const
{
   return m_elems.size();
}

///////////////////////////////////////////////////////////////////////////////

DataSource& DSIndexedBindStructure::getSource( const std::string& semantic )
{
   return *m_sourcesDB.getSource( m_inputs[ semantic ]->source );
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DSIndexedBindStructure::getIndicesCount( unsigned int dataIndex ) const
{
   return m_elems[ dataIndex ].size() / m_inputs.size();
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DSIndexedBindStructure::getIndex( unsigned int dataIndex, const std::string& semantic, unsigned int offset ) const
{
   std::map< std::string, Input* >::const_iterator it = m_inputs.find( semantic );
   if ( it != m_inputs.end() )
   {
      unsigned int addr = offset * m_inputs.size() + it->second->offset;
      return m_elems[ dataIndex ][ addr ];
   }
   else
   {
      throw std::out_of_range( "No such semantic defined" );
      return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
