#include "FSDirNode.h"
#include "ResourcesBrowser.h"


///////////////////////////////////////////////////////////////////////////////

FSDirNode::FSDirNode( FSTreeNode* parent, const std::string& nodeName, const Filesystem& fs )
: FSTreeNode( parent, nodeName, fs )
{
   // set the icon
   QString iconsDir = fs.getShortcut( "editorIcons" ).c_str();
   setIcon( 0, QIcon( iconsDir + "dirIcon.png" ) );

   // set the description
   unsigned int nameLen = m_fsNodeName.length();
   std::string nameWithoutSlash = m_fsNodeName.substr( 0, nameLen - 1 );
   setText( 0, nameWithoutSlash.c_str() );
   setText( 1, "DIR" );
}

///////////////////////////////////////////////////////////////////////////////

std::string FSDirNode::getRelativePath() const
{
   ASSERT ( parent(), "Directory node has to have a parent" );

   std::string path = dynamic_cast< FSTreeNode* >( parent() )->getRelativePath();
   path += m_fsNodeName;

   return path;
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* FSDirNode::getDescFactory( ResourcesBrowser& resourcesFactory )
{
   return resourcesFactory.getDescFactory();
}

///////////////////////////////////////////////////////////////////////////////

void FSDirNode::addNode( unsigned int typeIdx, ResourcesBrowser& resourcesFactory )
{
   std::string path = getRelativePath();
   return resourcesFactory.addNode( typeIdx, path );
}

///////////////////////////////////////////////////////////////////////////////

