#include "FSDirNode.h"
#include "FilesystemTree.h"
#include "core\ResourcesManager.h"
#include "core\Filesystem.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

FSDirNode::FSDirNode( FSTreeNode* parent, const std::string& nodeName )
   : FSTreeNode( parent )
   , m_fsNodeName( nodeName )
{
   ASSERT_MSG( m_fsNodeName.empty() ? true : m_fsNodeName.c_str()[ m_fsNodeName.length() - 1 ] == '/', "This is a directory node, thus it has to end with a slash" );

   // strip the name of the last slash
   if ( !m_fsNodeName.empty() )
   {
      m_fsNodeName = m_fsNodeName.substr( 0, m_fsNodeName.length() - 1 );
   }

   // set the icon
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   QString iconsDir = fs.getShortcut( "editorIcons" ).c_str();
   setIcon( 0, QIcon( iconsDir + "dirIcon.png" ) );

   // set the description
   setText( 0, m_fsNodeName.c_str() );
   setText( 1, "DIR" );
}

///////////////////////////////////////////////////////////////////////////////

std::string FSDirNode::getRelativePath() const
{
   ASSERT_MSG ( parent(), "Directory node has to have a parent" );

   std::string path = dynamic_cast< FSTreeNode* >( parent() )->getRelativePath();
   path += m_fsNodeName + "/";

   return path;
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* FSDirNode::getDescFactory( FilesystemTree& resourcesFactory )
{
   return resourcesFactory.getDescFactory();
}

///////////////////////////////////////////////////////////////////////////////

void FSDirNode::addNode( unsigned int typeIdx, FilesystemTree& resourcesFactory )
{
   std::string path = getRelativePath();
   return resourcesFactory.addNode( typeIdx, path );
}

///////////////////////////////////////////////////////////////////////////////

void FSDirNode::openItem( FilesystemTree& hostTree )
{
   std::string thisDir = getRelativePath();
   hostTree.refresh( thisDir );
}

///////////////////////////////////////////////////////////////////////////////

bool FSDirNode::compareNodeName( const std::string& name ) const
{
   return name == m_fsNodeName;
}

///////////////////////////////////////////////////////////////////////////////
