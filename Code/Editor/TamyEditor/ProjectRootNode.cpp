#include "ProjectRootNode.h"
#include "ProjectTree.h"


///////////////////////////////////////////////////////////////////////////////

ProjectRootNode::ProjectRootNode( QTreeWidget* parent )
   : ProjectTreeNode( parent )
{
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();

   QString iconsDir = fs.getShortcut( "editorIcons" ).c_str();
   setIcon( 0, QIcon( iconsDir + "dirIcon.png" ) );

   setText( 0, "Root" );
   setText( 1, "" );
}

///////////////////////////////////////////////////////////////////////////////

std::string ProjectRootNode::getRelativePath() const
{
   return "/";
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* ProjectRootNode::getDescFactory( ProjectTree& tree )
{
   // no items can be created in project's root
   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ProjectRootNode::addNode( unsigned int typeIdx, ProjectTree& tree )
{
   std::string path = getRelativePath();
   return tree.addNode( typeIdx, path );
}

///////////////////////////////////////////////////////////////////////////////

bool ProjectRootNode::compareNodeName( const std::string& name ) const
{
   return name.empty() || name == "/";
}

///////////////////////////////////////////////////////////////////////////////
