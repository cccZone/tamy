#ifndef _COLLADA_PARSER_H
#error "This file can only be included form ColladaParser.h"
#else


///////////////////////////////////////////////////////////////////////////////

template< typename T >
TSourceCreator< T >::TSourceCreator( const std::string& postfix )
   : m_postfix( postfix )
{}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
std::string TSourceCreator< T >::createDefaultName( const std::string& baseName ) const
{
   return baseName + m_postfix;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
DataSource* TSourceCreator< T >::instantiate( TiXmlElement& elem, const ColladaParser& sourcesDB ) const
{
   return new T( elem, sourcesDB );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void ColladaParser::associate( const std::string& tag, const std::string& postfix )
{
   AdditionalSourcesMap::iterator it = m_additionalSources.find( tag );
   if ( it == m_additionalSources.end() )
   {
      it = m_additionalSources.insert( std::make_pair( tag, std::vector< SourceCreator* >() ) ).first;
   }

   std::vector< SourceCreator* >& creatorsArr = it->second;
   creatorsArr.push_back( new TSourceCreator< T >( postfix ) );
}

///////////////////////////////////////////////////////////////////////////////

#endif // _COLLADA_PARSER_H 
