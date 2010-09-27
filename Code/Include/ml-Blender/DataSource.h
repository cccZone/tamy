/// @file   ml-Blender/DataInput.h
/// @brief  COLLADA data source description

#pragma once

#include <d3dx9.h>
#include <vector>


///////////////////////////////////////////////////////////////////////////////

class TiXmlElement;

///////////////////////////////////////////////////////////////////////////////

class DataSource
{
public:
   virtual ~DataSource() {}

   virtual unsigned int size() const = 0;

   virtual void readData( int dataOffset, void* data ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class IDSArrayType
{
public:
   virtual ~IDSArrayType() {}

   virtual void readData( int dataOffset, void* data ) = 0;
};

///////////////////////////////////////////////////////////////////////////////

class DSArray : public DataSource
{
private:
   unsigned int                     m_count;
   IDSArrayType*                    m_type;

public:
   DSArray( TiXmlElement& elem );
   ~DSArray();

   // -------------------------------------------------------------------------
   // DataSource implementation
   // -------------------------------------------------------------------------
   unsigned int size() const { return m_count; }
   void readData( int dataOffset, void* data );
};

///////////////////////////////////////////////////////////////////////////////
