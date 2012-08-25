#include "core\IDString.h"
#include "stdio.h"


///////////////////////////////////////////////////////////////////////////////

IDString::IDString( uint stringId )
   : m_stringId( stringId )
{
}

///////////////////////////////////////////////////////////////////////////////

IDString::IDString( const char* str )
{
   m_stringId = IDStringsPool::getInstance().registerString( str );
}

///////////////////////////////////////////////////////////////////////////////

IDString::IDString( const std::string& str )
{
   m_stringId = IDStringsPool::getInstance().registerString( str.c_str() );
}

///////////////////////////////////////////////////////////////////////////////

IDString::IDString( const IDString& rhs )
   : m_stringId( rhs.m_stringId )
{
}

///////////////////////////////////////////////////////////////////////////////

bool IDString::operator==( const IDString& rhs ) const
{
   return m_stringId == rhs.m_stringId;
}

///////////////////////////////////////////////////////////////////////////////

bool IDString::operator!=( const IDString& rhs ) const
{
   return m_stringId != rhs.m_stringId;
}

///////////////////////////////////////////////////////////////////////////////

IDString& IDString::operator=( const IDString& rhs )
{
   m_stringId = rhs.m_stringId;
   return *this;
}

///////////////////////////////////////////////////////////////////////////////

const char* IDString::c_str() const
{
   IDStringsPool& pool = IDStringsPool::getInstance();
   const char* str = pool.getString( m_stringId );

   return str;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

IDStringsPool IDStringsPool::s_theInstance;

///////////////////////////////////////////////////////////////////////////////

IDStringsPool::IDStringsPool()
   : m_strings( 1024 )
{
}

///////////////////////////////////////////////////////////////////////////////

IDStringsPool::~IDStringsPool()
{
   uint count = m_strings.size();
   for ( uint i = 0; i < count; ++i )
   {
      char* str = m_strings[i];
      delete [] str;
   }
   m_strings.clear();
}

///////////////////////////////////////////////////////////////////////////////

uint IDStringsPool::registerString( const char* str )
{
   // check if the string is already registered
   uint count = m_strings.size();
   for ( uint i = 0; i < count; ++i )
   {
      if ( strcmp( m_strings[i], str ) == 0 )
      {
         // found it
         return i;
      }
   }

   // it's a new string - add it
   uint strLength = strlen( str );
   char* strCopy = new char[strLength + 1];
   strcpy( strCopy, str );

   uint stringId = m_strings.size();
   m_strings.push_back( strCopy );

   return stringId;
}

///////////////////////////////////////////////////////////////////////////////

const char* IDStringsPool::getString( uint stringId ) const
{
   return m_strings[stringId];
}

///////////////////////////////////////////////////////////////////////////////
