#include "FSTreeItemsFactory.h"
#include <QIcon>


///////////////////////////////////////////////////////////////////////////////

FSTreeItemsFactory::FSTreeItemsFactory( const QString& iconsDir )
   : m_iconsDir( iconsDir )
{
}

///////////////////////////////////////////////////////////////////////////////

unsigned int FSTreeItemsFactory::typesCount() const
{
   return 2;
}

///////////////////////////////////////////////////////////////////////////////

void FSTreeItemsFactory::getDesc( unsigned int idx, QString& outDesc, QIcon& outIcon ) const
{
   switch( idx )
   {
      case FS_ITEM_DIR:
      {
         outDesc = "Directory";
         outIcon = QIcon( m_iconsDir + "dirIcon.png" );

         break;
      }

      case FS_ITEM_PROJECT:
      {
         outDesc = "Project";
         outIcon = QIcon( m_iconsDir + "ProjectIcon.png" );
         break;
      }
   }
}

///////////////////////////////////////////////////////////////////////////////

void FSTreeItemsFactory::getDesc( const SerializableReflectionType& type, QString& outDesc, QIcon& outIcon ) const
{
}

///////////////////////////////////////////////////////////////////////////////

const SerializableReflectionType* FSTreeItemsFactory::getClass( unsigned int idx ) const
{
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

