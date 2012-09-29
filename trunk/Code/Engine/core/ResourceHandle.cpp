#include "core/ResourceHandle.h"
#include "core/Resource.h"


///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( ResourceHandle );
   PARENT( ReflectionObject);
   PROPERTY( std::string, m_resourcePath );
   PROPERTY( int, m_objectId )
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

ResourceHandle::ResourceHandle( const std::string& resourcePath, int objectId )
   : m_resourcePath( resourcePath )
   , m_objectId( objectId )
{

}

///////////////////////////////////////////////////////////////////////////////

void ResourceHandle::onObjectPreSave()
{

}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

BEGIN_OBJECT( ResourceObject );
   PARENT( ReflectionObject);
   PROPERTY( TRefPtr< Resource >, m_hostResource );
   PROPERTY( int, m_objectId );
END_OBJECT();

///////////////////////////////////////////////////////////////////////////////

ResourceObject::ResourceObject()
   : m_hostResource( NULL )
   , m_objectId( -1 )
{
}

///////////////////////////////////////////////////////////////////////////////

ResourceObject::~ResourceObject()
{
   m_hostResource = NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ResourceObject::setHostResource( Resource& hostResource, int objectId )
{
   if ( m_hostResource.isNotNull() )
   {
      ASSERT_MSG( false, "The object is already managed by a resource" );
   }
   m_hostResource = &hostResource;
   m_objectId = objectId;
}

///////////////////////////////////////////////////////////////////////////////

void ResourceObject::resetHostResource()
{
   if ( m_hostResource.isNull() )
   {
      ASSERT_MSG( false,  "The object is not managed by any resource" );
   }
   m_hostResource = NULL;
   m_objectId = -1;
}

///////////////////////////////////////////////////////////////////////////////

