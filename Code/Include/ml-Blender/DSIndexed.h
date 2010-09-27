/// @file   ml-Blender/DSIndexed.h
/// @brief  a COLLADA data structure with indexed array

#pragma once

#include "ml-Blender/DataSource.h"


///////////////////////////////////////////////////////////////////////////////

class ColladaParser;

///////////////////////////////////////////////////////////////////////////////

class DSIndexed : public DataSource
{
private:
   std::vector< unsigned int >            m_startIndices;
   std::vector< unsigned int >            m_counts;

public:
   DSIndexed( TiXmlElement& elem, const ColladaParser& sourcesDB );

   // -------------------------------------------------------------------------
   // DataSource implementation
   // -------------------------------------------------------------------------
   unsigned int size() const;
   void readData( int dataOffset, void* data );

protected:
   virtual void onReadData( int startIdx, int count, void* data ) = 0;
};

///////////////////////////////////////////////////////////////////////////////
