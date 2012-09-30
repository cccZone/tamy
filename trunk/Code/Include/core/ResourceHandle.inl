#ifndef _RESOURCE_HANDLE_H
#error "This file can only be included from ResourceHandle.h"
#else

#include "core/ResourcesManager.h"
#include "core/Resource.h"


///////////////////////////////////////////////////////////////////////////////

template< typename T >
TResourceHandle< T >::TResourceHandle( ResourceObject& object )
   : ResourceHandle( 
   object.hasHostResource() ? object.getHostResource().getFilePath().getRelativePath() : NULL, 
   object.getObjectId() )
{

}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
TResourceHandle< T >::TResourceHandle( const std::string& resourcePath, int objectId )
   : ResourceHandle( resourcePath, objectId )
{

}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
T& TResourceHandle< T >::get()
{
   Resource* resource = ResourcesManager::getInstance().findResource( m_resourcePath );
   if ( resource == NULL )
   {
      char msg[512];
      sprintf_s( msg, "Resource '%s' not found", m_resourcePath.c_str() );
      ASSERT_MSG( resource != NULL, msg );
   }

   ResourceObject& obj = resource->getObject( m_objectId );
   T* tObj = dynamic_cast< T* >( &obj );
   if ( tObj == NULL )
   {
      char msg[512];
      sprintf_s( msg, "Object '%d' from resource '%s' doesn't match the expected type", m_objectId, m_resourcePath.c_str() );
      ASSERT_MSG( tObj != NULL, msg );
   }

   return *tObj;
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const T& TResourceHandle< T >::get() const
{
   Resource* resource = ResourcesManager::getInstance().findResource( m_resourcePath );
   if ( resource == NULL )
   {
      char msg[512];
      sprintf_s( msg, "Resource '%s' not found", m_resourcePath.c_str() );
      ASSERT_MSG( resource != NULL, msg );
   }

   ResourceObject& obj = resource->getObject( m_objectId );
   T* tObj = dynamic_cast< T* >( &obj );
   if ( tObj == NULL )
   {
      char msg[512];
      sprintf_s( msg, "Object '%d' from resource '%s' doesn't match the expected type", m_objectId, m_resourcePath.c_str() );
      ASSERT_MSG( tObj != NULL, msg );
   }

   return *tObj;
}

///////////////////////////////////////////////////////////////////////////////

bool ResourceObject::hasHostResource() const
{
   return m_hostResource != NULL;
}

///////////////////////////////////////////////////////////////////////////////

Resource& ResourceObject::getHostResource()
{
   return *m_hostResource;
}

///////////////////////////////////////////////////////////////////////////////

int ResourceObject::getObjectId()
{
   return m_objectId;
}

///////////////////////////////////////////////////////////////////////////////

#endif // _RESOURCE_HANDLE_H
