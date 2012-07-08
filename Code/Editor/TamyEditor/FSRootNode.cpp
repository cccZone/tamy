#include "FSRootNode.h"
#include "FilesystemTree.h"


///////////////////////////////////////////////////////////////////////////////

FSRootNode::FSRootNode( QTreeWidget* parent )
   : FSTreeNode( parent )
   , m_fsNodeName( "/" )
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   QString iconsDir = fs.getShortcut( "editorIcons" ).c_str();
   setIcon( 0, QIcon( iconsDir + "dirIcon.png" ) );

   setText( 0, "Root" );
   setText( 1, "" );
}

///////////////////////////////////////////////////////////////////////////////

std::string FSRootNode::getRelativePath() const
{
   return "/";
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* FSRootNode::getDescFactory( FilesystemTree& resourcesFactory )
{
   return resourcesFactory.getDescFactory();
}

///////////////////////////////////////////////////////////////////////////////

void FSRootNode::addNode( unsigned int typeIdx, FilesystemTree& resourcesFactory )
{
   std::string path = getRelativePath();
   return resourcesFactory.addNode( typeIdx, path );
}

///////////////////////////////////////////////////////////////////////////////

bool FSRootNode::compareNodeName( const std::string& name ) const
{
   return name.empty() || name == m_fsNodeName;
}

///////////////////////////////////////////////////////////////////////////////
