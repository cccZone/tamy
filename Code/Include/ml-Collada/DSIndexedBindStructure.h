/// @file   ml-Collada/DSBindStructure.h
/// @brief  a COLLADA data structure for data described by multiple inputs with indexing

#pragma once

#include "ml-Collada/DataSource.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class ColladaParser;

///////////////////////////////////////////////////////////////////////////////

class DSIndexedBindStructure : public DataSource
{
private:
   struct Input
   {
      unsigned int                                 offset;
      std::string                                  source;

      Input( unsigned int _offset, const std::string& _source )
         : offset( _offset ), source( _source )
      {}
   };

private:
   const ColladaParser&                            m_sourcesDB;
   std::map< std::string, Input* >                 m_inputs;
   std::vector< std::vector< unsigned int > >      m_elems;

public:
   /**
    * Constructor.
    */
   DSIndexedBindStructure( TiXmlElement& elem, const ColladaParser& sourcesDB );
   ~DSIndexedBindStructure();

   // -------------------------------------------------------------------------
   // DataSource implementation
   // -------------------------------------------------------------------------
   unsigned int size() const;

protected:
   unsigned int getIndicesCount( unsigned int dataIndex ) const;

   unsigned int getIndex( unsigned int dataIndex, const std::string& semantic, unsigned int offset ) const;

   DataSource& getSource( const std::string& semantic );
};

///////////////////////////////////////////////////////////////////////////////
