#include "ml-Collada/DSBindStructure.h"
#include "ml-Collada/ColladaParser.h"
#include "tinyxml/tinyxml.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

DSBindStructure::DSBindStructure( TiXmlElement& elem, const ColladaParser& sourcesDB )
   : m_sourcesDB( sourcesDB )
{
   // parse the structure
   for ( TiXmlElement* inputElem = elem.FirstChildElement( "input" ); inputElem != NULL; inputElem = inputElem->NextSiblingElement( "input" ) ) 
   {
      std::string semantic = inputElem->Attribute( "semantic" );
      std::string source = inputElem->Attribute( "source" );
      source = source.substr( 1 );

      m_inputs.insert( std::make_pair( semantic, source ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

DSBindStructure::~DSBindStructure()
{
   m_inputs.clear();
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DSBindStructure::size() const
{
   if ( m_inputs.empty() )
   {
      return 0;
   }
   else
   {
      return m_sourcesDB.getSource( m_inputs.begin()->second )->size();
   }
}

///////////////////////////////////////////////////////////////////////////////

DataSource& DSBindStructure::getSource( const std::string& semantic )
{
   return *m_sourcesDB.getSource( m_inputs[ semantic ] );
}

///////////////////////////////////////////////////////////////////////////////
