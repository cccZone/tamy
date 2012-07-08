#ifndef _TYPE_DESC_FACTORY_H
#error "This file can only be included from TypeDescFactory.h"
#else

#include "core\Filesystem.h"
#include <QIcon.h>


///////////////////////////////////////////////////////////////////////////////

template< typename T >
TypeDescFactory< T >::TypeDescFactory( const QString& iconsDir, const Filesystem& fs, const QString& defaultIcon ) 
: m_iconsDir( iconsDir )
, m_fs( fs )
, m_defaultIcon( defaultIcon )
{
   ReflectionTypesRegistry& typesReg = ReflectionTypesRegistry::getInstance();
   typesReg.getMatchingSerializableTypes< T >( m_classes, false );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T > template< typename TypeToRemove >
void TypeDescFactory< T >::removeType()
{
    const SerializableReflectionType& type = TypeToRemove::getStaticRTTI();

    uint count = m_classes.size();
    for ( uint i = 0; i < count; ++i )
    {
       if ( m_classes[i]->isExactlyA( type ) )
       {
          // found it - remove it and exit
          m_classes.erase( m_classes.begin() + i );
          break;
       }
    }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TypeDescFactory< T >::typesCount() const 
{
   return m_classes.size();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TypeDescFactory< T >::getDesc( unsigned int idx,  QString& outDesc, QIcon& outIcon ) const
{
   const SerializableReflectionType& type = *m_classes[ idx ];
   getDesc( type, outDesc, outIcon );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TypeDescFactory< T >::getDesc( const SerializableReflectionType& type, QString& outDesc, QIcon& outIcon ) const
{
   // get the name of the type
   outDesc = type.m_name.c_str();

   // find the first applicable icon there is for this type
   std::list< const SerializableReflectionType* > typesQueue;
   typesQueue.push_back( &type );
   QString validIconName;
   while ( !typesQueue.empty() )
   {
      const SerializableReflectionType& currType = *typesQueue.front();
      std::string iconName = getIconName( currType ).toStdString();
      if ( m_fs.doesExist( m_fs.toRelativePath( iconName ) ) )
      {
         validIconName = iconName.c_str();
         break;
      }

      std::vector< const SerializableReflectionType* > parents;
      currType.collectParents( parents );
      unsigned int parentsCount = parents.size();
      for ( unsigned int i = 0; i < parentsCount; ++i )
      {
         typesQueue.push_back( parents[ i ] );
      }

      typesQueue.pop_front();
   }

   if ( !validIconName.isEmpty() )
   {
      outIcon = QIcon( validIconName );
   }
   else
   {
      outIcon = QIcon( m_iconsDir + m_defaultIcon );
   }
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
const SerializableReflectionType* TypeDescFactory< T >::getClass( unsigned int idx ) const
{
   return m_classes[ idx ];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
QString TypeDescFactory< T >::getIconName( const SerializableReflectionType& type ) const 
{
   return m_iconsDir + type.m_name.c_str() + "Icon.png";
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TYPE_DESC_FACTORY_H
