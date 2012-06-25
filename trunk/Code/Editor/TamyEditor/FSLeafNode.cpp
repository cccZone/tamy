#include "FSLeafNode.h"
#include "ResourcesBrowser.h"
#include <stdexcept>
#include <QFileIconProvider>
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

FSLeafNode::FSLeafNode( FSTreeNode* parent, const std::string& nodeName, const Filesystem& fs, TypeDescFactory< Resource >& itemsFactory )
   : FSTreeNode( parent, fs )
   , m_fsNodeName( nodeName )
{
   ASSERT_MSG( m_fsNodeName.empty() ? true : m_fsNodeName.c_str()[ m_fsNodeName.length() - 1 ] != '/', "This is a leaf node, dedicated to a file, thus it can't end with a slash" );

   // set icon
   setEntryIcon( fs, itemsFactory );

   // set description
   setText( 0, m_fsNodeName.c_str() );
   setEntrySize( fs );
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::setEntryIcon( const Filesystem& fs, TypeDescFactory< Resource >& itemsFactory )
{
   std::string iconsDir = fs.getShortcut( "editorIcons" );
   std::string extension = Filesystem::extractExtension( m_fsNodeName );

   QIcon icon;

   const SerializableReflectionType* resourceType = Resource::findResourceClass( extension.c_str() );
   if ( resourceType )
   {
      // as the first shot, try creating an icon corresponding to the type of the resource
      QString typeName;
      itemsFactory.getDesc( *resourceType, typeName, icon );
   }
   
   if ( icon.isNull() )
   {
      // if the resource is of an unknown type, try finding an icon matching the file extension
      std::string iconName = iconsDir + "/" + extension + "Icon.png";
      if ( fs.doesExist( fs.toRelativePath( iconName ) ) )
      {
         icon = QIcon( iconName.c_str() );
      }
   }

   if ( icon.isNull() )
   {
      // as a last resort, try using a system icon
      QFileIconProvider iconProvider;

      std::string absolutePath = fs.toAbsolutePath( getRelativePath() );
      QFileInfo fileInfo( absolutePath.c_str() );
      icon = iconProvider.icon( fileInfo );
   }

   setIcon( 0, icon );
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::setEntrySize( const Filesystem& fs )
{
   std::string relativePath = getRelativePath();

   std::size_t fileSize = 0;
   try
   {
      File* file = fs.open( relativePath );
      fileSize = file->size();
      delete file;
   }
   catch ( std::exception& ex )
   {
      fileSize = 0;
   }

   char fileSizeStr[ 32 ];
   sprintf_s( fileSizeStr, "%d", fileSize );
   setText( 1, fileSizeStr );
}

///////////////////////////////////////////////////////////////////////////////

std::string FSLeafNode::getRelativePath() const
{
   ASSERT_MSG ( parent(), "Leaf node has to have a parent" );

   std::string path = dynamic_cast< FSTreeNode* >( parent() )->getRelativePath();
   path += m_fsNodeName;

   return path;
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::editResource( ResourcesBrowser& editorsFactory )
{
   std::string pathToResource = getRelativePath();
   editorsFactory.editResource( pathToResource, icon(0) );
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* FSLeafNode::getDescFactory( ResourcesBrowser& resourcesFactory )
{
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FSLeafNode::addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory )
{
   throw std::logic_error( "This operation should never be performed" );
}

///////////////////////////////////////////////////////////////////////////////

bool FSLeafNode::compareNodeName( const std::string& name ) const
{
   return name == m_fsNodeName;
}

///////////////////////////////////////////////////////////////////////////////
