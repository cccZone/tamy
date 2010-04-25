#include "core\Serializer.h"
#include "core\SerializerImpl.h"
#include "core\Serializable.h"
#include "core\Object.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

DependenciesMapper::DependenciesMapper( DependenciesSet& dependencies, PointerSaveState state )
: m_dependencies( dependencies )
, m_state( state )
{
}

///////////////////////////////////////////////////////////////////////////////

void DependenciesMapper::operator<<(std::string& str)
{
}

///////////////////////////////////////////////////////////////////////////////

void DependenciesMapper::operator<<(D3DXMATRIX& mtx)
{
}

///////////////////////////////////////////////////////////////////////////////

void DependenciesMapper::operator<<(D3DXVECTOR3& vec)
{
}

///////////////////////////////////////////////////////////////////////////////

void DependenciesMapper::operator<<(Serializable** ptr)
{
   if ( m_state == PSS_SAVE )
   {
      unsigned int count = m_dependencies.size();
      for (unsigned int idx = 0; idx < count; ++idx)
      {
         if (m_dependencies[idx] == *ptr)
         {
            // yes - we have it mapped
            return;
         }
      }

      m_dependencies.push_back( *ptr );
      (*ptr)->save( *this );
   }
   else if ( m_state == PSS_UPDATE )
   {
      unsigned int idx = reinterpret_cast< unsigned int >( *ptr );
      *ptr = m_dependencies[ idx ];
   }
}

///////////////////////////////////////////////////////////////////////////////

void DependenciesMapper::serializeBuf(byte* buf, size_t size)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Saver::Saver( SerializerImpl* impl )
: m_impl( impl )
{
}

///////////////////////////////////////////////////////////////////////////////

Saver::~Saver()
{
   delete m_impl;
   m_impl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Saver::save( Serializable& serializable )
{
   // map the dependencies
   m_dependencies.clear();
   m_dependencies.push_back( NULL );

   // dependency 1 is always going to be the serializable we want to save
   m_dependencies.push_back( &serializable );

   DependenciesMapper mapper( m_dependencies, PSS_SAVE );
   serializable.save( mapper );

   // save the objects recovered during the dependencies mapping
   unsigned int dependenciesToSaveCount = m_dependencies.size() - 1;
   m_impl->write( ( byte* )&dependenciesToSaveCount, sizeof( unsigned int ) );

   // We'll save from the first dependency - 'cause we know
   // the dependency 0 is the NULL pointer.
   unsigned int count = m_dependencies.size();
   for ( unsigned int i = 1; i < count; ++i )
   {
      m_dependencies[ i ]->save( *this );
   }
}

///////////////////////////////////////////////////////////////////////////////

void Saver::serializeBuf(byte* buf, size_t size)
{
   m_impl->write(buf, size);
}

///////////////////////////////////////////////////////////////////////////////

void Saver::operator<<(std::string& str)
{
   unsigned int strLen = str.length();
   m_impl->write((byte*)&strLen, sizeof(unsigned int));
   m_impl->write((byte*)str.c_str(), sizeof(char) * strLen);
}

///////////////////////////////////////////////////////////////////////////////

void Saver::operator<<(D3DXMATRIX& mtx)
{
   m_impl->write((byte*)&(mtx.m), sizeof(float) * 16);
}

///////////////////////////////////////////////////////////////////////////////

void Saver::operator<<(D3DXVECTOR3& vec)
{
   m_impl->write((byte*)&(vec.x), sizeof(float));
   m_impl->write((byte*)&(vec.y), sizeof(float));
   m_impl->write((byte*)&(vec.z), sizeof(float));
}

///////////////////////////////////////////////////////////////////////////////

void Saver::operator<<(Serializable** ptr)
{
   unsigned int count = m_dependencies.size();
   unsigned int idx = 0;

   // check if we have the dependency mapped
   for (; idx < count; ++idx)
   {
      if (m_dependencies[idx] == *ptr)
      {
         m_impl->write((byte*)&idx, sizeof(idx));
         return;
      }
   }

   ASSERT( false, "Unmapped dependency encountered" );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Loader::Loader(SerializerImpl* impl)
: m_impl(impl)
{
}

///////////////////////////////////////////////////////////////////////////////

Loader::~Loader()
{
   delete m_impl;
   m_impl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Loader::serializeBuf(byte* buf, size_t size)
{
   std::size_t bytesRead = m_impl->read( buf, size );
   ASSERT((bytesRead == size), "Error reading a binary archive");
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(std::string& str)
{
   unsigned int strLen = 0;
   std::size_t bytesRead = m_impl->read((byte*)&strLen, sizeof(unsigned int));
   ASSERT( ( bytesRead == sizeof( unsigned int ) ), "Error reading a binary archive" );

   char* strBuf = new char[strLen + 1];
   bytesRead = m_impl->read((byte*)strBuf, sizeof(char) * strLen);
   strBuf[strLen] = 0;
   str = strBuf;
   delete [] strBuf;
   ASSERT( ( bytesRead == strLen * sizeof( char ) ), "Error reading a binary archive" );
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(D3DXMATRIX& mtx)
{
   std::size_t bytesToRead = 16 * sizeof( float );
   std::size_t bytesRead = m_impl->read((byte*)&(mtx.m), bytesToRead);

   ASSERT( ( bytesRead == bytesToRead ), "Error reading a binary archive" );
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(D3DXVECTOR3& vec)
{
   std::size_t bytesRead = m_impl->read((byte*)&(vec.x), sizeof(float));
   bytesRead += m_impl->read((byte*)&(vec.y), sizeof(float));
   bytesRead += m_impl->read((byte*)&(vec.z), sizeof(float));

   ASSERT( ( bytesRead == 3 * sizeof( float ) ), "Error reading a binary archive" );
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(Serializable** ptr)
{
   unsigned int idx = 0;
   m_impl->read((byte*)&idx, sizeof(idx));
   *ptr = reinterpret_cast< Serializable*>( idx );
}

///////////////////////////////////////////////////////////////////////////////
