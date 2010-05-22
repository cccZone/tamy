#ifndef _TYPE_DESC_FACTORY_H
#error "This file can only be included from TypeDescFactory.h"
#else

#include "core\Filesystem.h"
#include <QIcon.h>


///////////////////////////////////////////////////////////////////////////////

template< typename T >
TypeDescFactory< T >::TypeDescFactory( const QString& iconsDir, 
                                       const Filesystem& fs,
                                       const QString& defaultIcon ) 
: m_iconsDir( iconsDir )
, m_fs( fs )
, m_defaultIcon( defaultIcon )
{
   ClassesRegistry& classesReg = getClassesRegistry();
   classesReg.getClassesMatchingType< T >( m_classes );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
unsigned int TypeDescFactory< T >::typesCount() const 
{
   return m_classes.size();
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TypeDescFactory< T >::getDesc( unsigned int idx, 
                                    QString& outDesc,
                                    QIcon& outIcon ) const
{
   const Class& type = m_classes[ idx ];
   getDesc( type, outDesc, outIcon );
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
void TypeDescFactory< T >::getDesc( const Class& type, 
                                    QString& outDesc, 
                                    QIcon& outIcon ) const
{
   // get the name of the type
   outDesc = type.getShortName().c_str();

   // find the first applicable icon there is for this type
   std::list< Class > typesQueue;
   typesQueue.push_back( type );
   QString validIconName;
   while ( !typesQueue.empty() )
   {
      const Class& currType = typesQueue.front();
      QString iconName = getIconName( currType );
      if ( m_fs.doesExist( m_fs.toRelativePath( iconName.toStdString() ) ) )
      {
         validIconName = iconName;
         break;
      }

      std::vector< Class > parents;
      currType.getParents( parents );
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
const Class& TypeDescFactory< T >::getClass( unsigned int idx ) const
{
   return m_classes[ idx ];
}

///////////////////////////////////////////////////////////////////////////////

template< typename T >
QString TypeDescFactory< T >::getIconName( const Class& type ) const 
{
   return m_iconsDir + type.getShortName().c_str() + "Icon.png";
}

///////////////////////////////////////////////////////////////////////////////

#endif // _TYPE_DESC_FACTORY_H
