#include "ml-Collada/ColladaParser.h"
#include "ml-Collada/DataSource.h"
#include "tinyxml/tinyxml.h"
#include "core.h"


///////////////////////////////////////////////////////////////////////////////

ColladaParser::ColladaParser( TiXmlElement& elem )
   : m_elem( elem )
{
}

///////////////////////////////////////////////////////////////////////////////

ColladaParser::~ColladaParser()
{
   for ( SourcesMap::iterator it = m_sources.begin(); it != m_sources.end(); ++it )
   {
      delete it->second;
   }
   m_sources.clear();

   for ( AdditionalSourcesMap::iterator it = m_additionalSources.begin(); it != m_additionalSources.end(); ++it )
   {
      std::vector< SourceCreator* >& creatorsArr = it->second;
      for ( std::vector< SourceCreator* >::iterator sIt = creatorsArr.begin(); sIt != creatorsArr.end(); ++sIt )
      {
         delete *sIt;
      }
   }
   m_additionalSources.clear();

   m_structures.clear();
}

///////////////////////////////////////////////////////////////////////////////

void ColladaParser::initialize()
{
   // parse regular sources
   for ( TiXmlElement* sourceElem = m_elem.FirstChildElement( "source" ); sourceElem != NULL; sourceElem = sourceElem->NextSiblingElement( "source" ) ) 
   {
      std::string sourceId = sourceElem->Attribute( "id" );
      m_sources.insert( std::make_pair( sourceId, new DSArray( *sourceElem ) ) );
   }

   // parse extra structures
   for ( AdditionalSourcesMap::iterator it = m_additionalSources.begin(); it != m_additionalSources.end(); ++it )
   {
      TiXmlElement* elem = m_elem.FirstChildElement( it->first.c_str() );
      if ( !elem )
      {
         continue;
      }

      std::vector< SourceCreator* >& sourcesArr = it->second;
      for ( std::vector< SourceCreator* >::iterator sIt = sourcesArr.begin(); sIt != sourcesArr.end(); ++sIt )
      {
         DataSource* source = (*sIt)->instantiate( *elem, *this );

         // register self
         const char* id = elem->Attribute( "id" );
         std::string idStr;
         if ( id )
         {
            idStr = id;
         }
         else
         {
            idStr = (*sIt)->createDefaultName( it->first );
         }
         m_sources.insert( std::make_pair( idStr, source ) );
         m_structures.insert( std::make_pair( it->first, source ) );
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

DataSource* ColladaParser::getSource( const std::string& sourceId ) const
{
   SourcesMap::const_iterator it = m_sources.find( sourceId );
   if ( it != m_sources.end() )
   {
      return it->second;
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////

DataSource* ColladaParser::getStructure( const std::string& structureId ) const
{
   SourcesMap::const_iterator it = m_structures.find( structureId );
   if ( it != m_structures.end() )
   {
      return it->second;
   }
   else
   {
      return NULL;
   }
}

///////////////////////////////////////////////////////////////////////////////
