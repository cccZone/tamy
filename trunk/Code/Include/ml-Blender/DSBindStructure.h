/// @file   ml-Blender/DSBindStructure.h
/// @brief  a COLLADA data structure for data described by multiple inputs

#pragma once

#include "ml-Blender/DataSource.h"
#include <map>


///////////////////////////////////////////////////////////////////////////////

class ColladaParser;

///////////////////////////////////////////////////////////////////////////////

class DSBindStructure : public DataSource
{
private:
   const ColladaParser&                                  m_sourcesDB;
   std::map< std::string, std::string >                  m_inputs;

public:
   /**
    * Constructor.
    */
   DSBindStructure( TiXmlElement& elem, const ColladaParser& sourcesDB );
   ~DSBindStructure();

   // -------------------------------------------------------------------------
   // DataSource implementation
   // -------------------------------------------------------------------------
   unsigned int size() const;

protected:
   DataSource& getSource( const std::string& semantic );
};

///////////////////////////////////////////////////////////////////////////////
