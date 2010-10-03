#ifndef _BLENDER_SCENE_H
#error "This file can only be included from BlenderScene.h"
#else

#include "tinyxml/tinyxml.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

template < typename T >
TSliceDefinition< T >::TSliceDefinition( const std::string& sliceTag, const std::string& sectionTag )
   : m_sliceTag( sliceTag )
   , m_sectionTag( sectionTag )
{
}

///////////////////////////////////////////////////////////////////////////////

template < typename T >
void TSliceDefinition< T >::parse( TiXmlNode* sceneSlice, BlenderScene& host )
{
   TiXmlElement* objectsSlice = sceneSlice->FirstChildElement( m_sliceTag.c_str() );
   if ( !objectsSlice )
   {
      return;
   }

   // initialize the observer
   unsigned int elemsCount = 0;
   for ( TiXmlNode* res = objectsSlice->FirstChild( m_sectionTag.c_str() ); res; res = res->NextSibling( m_sectionTag.c_str() ), ++elemsCount ) {}

   static char sliceId[128];
   sprintf_s( sliceId, "Loading slice '%s'", m_sliceTag.c_str() );
   host.getObserver().initialize( sliceId, elemsCount );

   // load the resources
   for ( TiXmlNode* resNode = objectsSlice->FirstChild( m_sectionTag.c_str() ); resNode; resNode = resNode->NextSibling( m_sectionTag.c_str() ) ) 
   {
      TiXmlElement* resElem = resNode->ToElement();
      ASSERT( resElem != NULL );

      std::string id = resElem->Attribute( "id" );
      IColladaSlice* slice = new T( resElem, host.getResourcesManager() );

      ISceneSlice* sceneSlice = dynamic_cast< ISceneSlice* >( slice );
      if ( sceneSlice )
      {
         sceneSlice->setID( id );
         host.addSceneSlice( sceneSlice );
      }
      else
      {
         host.addSlice( id, slice );
      }

      host.getObserver().advance();
   }
}

///////////////////////////////////////////////////////////////////////////////

template < typename T >
T& BlenderScene::getResource( const std::string& id ) const
{
   SlicesMap::const_iterator it = m_slicesMap.find( id );
   if ( it == m_slicesMap.end() )
   {
      throw std::runtime_error( "Slice doesn't exist" );
   }

   const IResourceSlice* resourceSlice = dynamic_cast< const IResourceSlice* >( it->second );
   if ( resourceSlice == NULL )
   {
      throw std::runtime_error( "Required slice is does not describe a resource" );
   }

   return resourceSlice->getResource< T >();
}

///////////////////////////////////////////////////////////////////////////////

#endif // _BLENDER_SCENE_H 
