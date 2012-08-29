#include "ml-Collada/DSIndexed.h"
#include "ml-Collada/ColladaParser.h"
#include "tinyxml/tinyxml.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

DSIndexed::DSIndexed( TiXmlElement& elem, const ColladaParser& sourcesDB )
{
   TiXmlElement* vcountElem = elem.FirstChildElement( "vcount" );
   ASSERT( vcountElem != NULL );
   std::string vcountData = vcountElem->GetText();
   std::stringstream vcountStr( vcountData );

   unsigned int entriesCount;
   elem.Attribute( "count", (int*)&entriesCount );

   unsigned int vCount, startIndices;
   startIndices = 0;
   for ( unsigned int i = 0; i < entriesCount; ++i )
   {
      vcountStr >> vCount;

      m_startIndices.push_back( startIndices );
      startIndices += vCount;

      m_counts.push_back( vCount );
   }
}

///////////////////////////////////////////////////////////////////////////////

unsigned int DSIndexed::size() const
{
   return m_counts.size();
}

///////////////////////////////////////////////////////////////////////////////

void DSIndexed::readData( int dataOffset, void* data )
{
   onReadData( m_startIndices[dataOffset], m_counts[dataOffset], data );
}

///////////////////////////////////////////////////////////////////////////////
