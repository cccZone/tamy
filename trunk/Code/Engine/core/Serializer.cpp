#include "core\Serializer.h"
#include "core\SerializerImpl.h"
#include "core\Serializable.h"
#include "core\Object.h"
#include "core\Resource.h"
#include "core\ResourcesManager.h"
#include "core\Assert.h"


///////////////////////////////////////////////////////////////////////////////

DependenciesMapper::DependenciesMapper( InternalDependenciesSet& internalDependencies,  
                                        PointerSaveState state )
: m_internalDependencies( internalDependencies )
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
      Serializable* serializable = *ptr;
      Resource* resource = dynamic_cast< Resource* >( serializable );
      if ( resource == NULL )
      {
         saveDependency< Serializable >( m_internalDependencies, serializable );
      }
   }
   else if ( m_state == PSS_UPDATE )
   {
      unsigned int idx = reinterpret_cast< unsigned int >( *ptr );
      if ( idx < m_internalDependencies.size() )
      {
         *ptr = m_internalDependencies[ idx ];
      }
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
, m_externalDependencies( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Saver::~Saver()
{
   delete m_impl;
   m_impl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void Saver::save( Resource& resource, ExternalDependenciesSet& outExternalDependencies )
{
   // map the dependencies
   m_externalDependencies = &outExternalDependencies;
   m_internalDependencies.clear();
   m_internalDependencies.push_back( NULL );

   // dependency 1 is always going to be the resource we want to save
   m_internalDependencies.push_back( &resource );

   DependenciesMapper mapper( m_internalDependencies, PSS_SAVE );
   resource.save( mapper );

   // save the objects recovered during the dependencies mapping
   unsigned int dependenciesToSaveCount = m_internalDependencies.size() - 1;
   m_impl->write( ( byte* )&dependenciesToSaveCount, sizeof( unsigned int ) );

   // We'll save from the first dependency - 'cause we know
   // the dependency 0 is the NULL pointer.
   unsigned int count = m_internalDependencies.size();
   for ( unsigned int i = 1; i < count; ++i )
   {
      m_internalDependencies[ i ]->save( *this );
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

void Saver::operator<<( Serializable** ptr )
{
   Serializable* serializable = *ptr;

   // check if that's an internal dependency - if so, save it's path
   byte checkedDepType = DPT_INTERNAL;
   unsigned int count = m_internalDependencies.size();
   for ( unsigned int idx = 0; idx < count; ++idx )
   {
      if (m_internalDependencies[idx] == serializable)
      {
         m_impl->write( ( byte* )&checkedDepType, sizeof( byte ) );
         m_impl->write( ( byte* )&idx, sizeof( idx ) );
         return;
      }
   }

   // check if that's an external dependency
   checkedDepType = DPT_EXTERNAL;
   Resource* resource = dynamic_cast< Resource* >( serializable );
   if ( resource != NULL )
   {
      // check if the dependency is not already mapped
      bool isDependencyMapped = false;
      count = m_externalDependencies->size();
      for ( unsigned int idx = 0; idx < count; ++idx )
      {
         if ( ( *m_externalDependencies )[idx] == resource )
         {
            isDependencyMapped = true;
            break;
         }
      }
      // map the dependency, if it's not been mapped yet
      if ( !isDependencyMapped )
      {
         m_externalDependencies->push_back( resource );
      }

      // store a link to the external dependency - it's file name
      m_impl->write( ( byte* )&checkedDepType, sizeof( byte ) );
      std::string filePath = resource->getFilePath();
      *this << filePath;
     
      return;
   }

   ASSERT_MSG( false, "Unmapped dependency encountered" );
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Loader::Loader(SerializerImpl* impl)
: m_impl(impl)
, m_resMgr( NULL )
{
}

///////////////////////////////////////////////////////////////////////////////

Loader::~Loader()
{
   delete m_impl;
   m_impl = NULL;
}

///////////////////////////////////////////////////////////////////////////////

Resource& Loader::load( ResourcesManager& resMgr )
{
   m_resMgr = &resMgr;

   // recover the dependencies
   m_dependencies.clear();
   m_dependencies.push_back( NULL );

   // load the dependencies
   unsigned int count = 0;
   m_impl->read( ( byte* )&count, sizeof( unsigned int ) );

   for ( unsigned int i = 0; i < count; ++i )
   {
      Serializable* ptr = Serializable::load<Serializable>(*this);
      m_dependencies.push_back( ptr );
   }

   // the root object is always the second one in the dependencies map
   // ('cause the first one is the NULL pointer)
   Serializable* root = m_dependencies[1];

   // update the dependencies between the objects
   DependenciesMapper mapper( m_dependencies, PSS_UPDATE );
   count = m_dependencies.size();
   for ( unsigned int i = 1; i < count; ++i )
   {
      m_dependencies[ i ]->save( mapper );
   }

   // notify the dependencies that they were successfully loaded
   for ( unsigned int i = 1; i < count; ++i )
   {
      m_dependencies[ i ]->onObjectLoaded();
   }

   // add the new resource to the resources manager
   Resource* resource = dynamic_cast< Resource* >( root );
   m_resMgr->addResource( resource );

   return *resource;
}

///////////////////////////////////////////////////////////////////////////////

void Loader::serializeBuf(byte* buf, size_t size)
{
   std::size_t bytesRead = m_impl->read( buf, size );
   ASSERT_MSG((bytesRead == size), "Error reading a binary archive");
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(std::string& str)
{
   unsigned int strLen = 0;
   std::size_t bytesRead = m_impl->read((byte*)&strLen, sizeof(unsigned int));
   ASSERT_MSG( ( bytesRead == sizeof( unsigned int ) ), "Error reading a binary archive" );

   char* strBuf = new char[strLen + 1];
   bytesRead = m_impl->read((byte*)strBuf, sizeof(char) * strLen);
   strBuf[strLen] = 0;
   str = strBuf;
   delete [] strBuf;
   ASSERT_MSG( ( bytesRead == strLen * sizeof( char ) ), "Error reading a binary archive" );
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(D3DXMATRIX& mtx)
{
   std::size_t bytesToRead = 16 * sizeof( float );
   std::size_t bytesRead = m_impl->read((byte*)&(mtx.m), bytesToRead);

   ASSERT_MSG( ( bytesRead == bytesToRead ), "Error reading a binary archive" );
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(D3DXVECTOR3& vec)
{
   std::size_t bytesRead = m_impl->read((byte*)&(vec.x), sizeof(float));
   bytesRead += m_impl->read((byte*)&(vec.y), sizeof(float));
   bytesRead += m_impl->read((byte*)&(vec.z), sizeof(float));

   ASSERT_MSG( ( bytesRead == 3 * sizeof( float ) ), "Error reading a binary archive" );
}

///////////////////////////////////////////////////////////////////////////////

void Loader::operator<<(Serializable** ptr)
{
   byte checkedDepType;
   m_impl->read( ( byte* )&checkedDepType, sizeof( byte ) );

   switch( checkedDepType )
   {
   case DPT_INTERNAL:
      {
         unsigned int idx = 0;
         m_impl->read((byte*)&idx, sizeof(idx));
         *ptr = reinterpret_cast< Serializable* >( idx );
         break;
      }

   case DPT_EXTERNAL:
      {
         std::string resourceFilePath;
         *this << resourceFilePath;
         *ptr = &( m_resMgr->create( resourceFilePath ) );

         break;
      }

   default:
      {
         ASSERT_MSG( false, "Invalid dependency type" );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////
