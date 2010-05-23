#include "FSRootNode.h"
#include "ResourcesBrowser.h"


///////////////////////////////////////////////////////////////////////////////

FSRootNode::FSRootNode( QTreeWidget* parent, const Filesystem& fs )
: FSTreeNode( parent, "/", fs )
{
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

TreeWidgetDescFactory* FSRootNode::getDescFactory( ResourcesBrowser& resourcesFactory )
{
   return resourcesFactory.getDescFactory();
}

///////////////////////////////////////////////////////////////////////////////

void FSRootNode::addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory )
{
   std::string path = getRelativePath();
   return resourcesFactory.addNode( typeIdx, path );
}

///////////////////////////////////////////////////////////////////////////////
