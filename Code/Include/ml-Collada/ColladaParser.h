/// @file   ml-Collada/ColladaParser.h
/// @brief  COLLADA data input

#ifndef _COLLADA_PARSER_H
#define _COLLADA_PARSER_H

#include <map>
#include <string>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class ColladaParser;
class DataSource;
class TiXmlElement;

///////////////////////////////////////////////////////////////////////////////

class SourceCreator
{
public:
   virtual ~SourceCreator() {}

   virtual std::string createDefaultName( const std::string& baseName ) const = 0;

   virtual DataSource* instantiate( TiXmlElement& elem, const ColladaParser& sourcesDB ) const = 0;
};

///////////////////////////////////////////////////////////////////////////////

template< typename T >
class TSourceCreator : public SourceCreator
{
private:
   std::string          m_postfix;

public:
   TSourceCreator( const std::string& postfix );

   // -------------------------------------------------------------------------
   // SourceCreator implementation
   // -------------------------------------------------------------------------
   std::string createDefaultName( const std::string& baseName ) const;

   DataSource* instantiate( TiXmlElement& elem, const ColladaParser& sourcesDB ) const;
};

///////////////////////////////////////////////////////////////////////////////

class ColladaParser
{
private:
   TiXmlElement&                                                  m_elem;

   typedef std::map< std::string, DataSource* >                   SourcesMap;
   SourcesMap                                                     m_sources;
   SourcesMap                                                     m_structures;

   typedef std::map< std::string, std::vector< SourceCreator* > > AdditionalSourcesMap;
   AdditionalSourcesMap                                           m_additionalSources;

public:
   /**
    * Constructor.
    */
   ColladaParser( TiXmlElement& elem );
   ~ColladaParser();

   /**
    * Associates an extra data structure with a data node.
    *
    * @param tag        data node tag
    * @param postfix    optional postfix to assign to a source name in case the node doesn't have an ID
    */
   template< typename T >
   void associate( const std::string& tag, const std::string& postfix = "" );

   /**
    * Parses a Collada document.
    */
   void initialize();
   
   /**
    * Returns a data source identified by the specified ID.
    *
    * @param sourceId
    */
   DataSource* getSource( const std::string& sourceId ) const;

   /**
    * Returns a data source structure identified by the specified ID.
    *
    * @param sourceId
    */
   DataSource* getStructure( const std::string& structureId ) const;
};

///////////////////////////////////////////////////////////////////////////////

#include "ml-Collada/ColladaParser.inl"

///////////////////////////////////////////////////////////////////////////////

#endif // _COLLADA_PARSER_H
