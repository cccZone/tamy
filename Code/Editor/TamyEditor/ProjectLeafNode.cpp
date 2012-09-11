#include "ProjectLeafNode.h"
#include "ProjectTree.h"

// tools
#include <QFileIconProvider>
#include "ResourceManagementUtil.h"
#include "TreeWidgetDescFactory.h"
#include "core/Assert.h"

// project management
#include "Project.h"
#include "tamyeditor.h"
#include "ResourcesBrowser.h"


///////////////////////////////////////////////////////////////////////////////

ProjectLeafNode::ProjectLeafNode( ProjectTreeNode* parent, const std::string& nodeName, TreeWidgetDescFactory& itemsFactory )
   : ProjectTreeNode( parent )
   , m_projectNodeName( nodeName )
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   ASSERT_MSG( m_projectNodeName.empty() ? true : m_projectNodeName.c_str()[ m_projectNodeName.length() - 1 ] != '/', "This is a leaf node, dedicated to a file, thus it can't end with a slash" );

   // set icon
   setEntryIcon( fs, itemsFactory );

   // set description
   setText( 0, m_projectNodeName.c_str() );
   setEntrySize( fs );
}

///////////////////////////////////////////////////////////////////////////////

void ProjectLeafNode::setEntryIcon( const Filesystem& fs, TreeWidgetDescFactory& itemsFactory )
{
   std::string iconsDir = fs.getShortcut( "editorIcons" );
   std::string extension = FilesystemUtils::extractExtension( m_projectNodeName );

   QIcon icon;

   const SerializableReflectionType* resourceType = Resource::findResourceClass( extension.c_str() );
   if ( resourceType )
   {
      // as the first shot, try creating an icon corresponding to the type of the resource
      QString typeName, group;
      itemsFactory.getDesc( *resourceType, typeName, group, icon );
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

void ProjectLeafNode::setEntrySize( const Filesystem& fs )
{
   std::string relativePath = getRelativePath();

   std::size_t fileSize = 0;
   File* file = fs.open( relativePath );
   if ( file )
   {
      fileSize = file->size();
      delete file;
   }
   else
   {
      fileSize = 0;
   }

   char fileSizeStr[ 32 ];
   sprintf_s( fileSizeStr, "%d", fileSize );
   setText( 1, fileSizeStr );
}

///////////////////////////////////////////////////////////////////////////////

std::string ProjectLeafNode::getRelativePath() const
{
   ASSERT_MSG ( parent(), "Leaf node has to have a parent" );

   std::string path = static_cast< ProjectTreeNode* >( parent() )->getRelativePath();
   path += m_projectNodeName;

   return path;
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* ProjectLeafNode::getDescFactory( ProjectTree& tree )
{
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ProjectLeafNode::addNode( unsigned int typeIdx, ProjectTree& tree )
{
   ASSERT_MSG( false, "This operation should never be performed" );
}

///////////////////////////////////////////////////////////////////////////////

bool ProjectLeafNode::compareNodeName( const std::string& name ) const
{
   return name == m_projectNodeName;
}

///////////////////////////////////////////////////////////////////////////////

void ProjectLeafNode::openItem( ProjectTree& tree )
{
   std::string pathToResource = getRelativePath();
   ResourceManagementUtil::editResource( pathToResource, icon(0) );
}

///////////////////////////////////////////////////////////////////////////////
