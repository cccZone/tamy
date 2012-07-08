#include "ProjectDirNode.h"
#include "ProjectTree.h"
#include "core\ResourcesManager.h"
#include "core\Filesystem.h"
#include "core/Assert.h"


///////////////////////////////////////////////////////////////////////////////

ProjectDirNode::ProjectDirNode( ProjectTreeNode* parent, const std::string& nodeName )
   : ProjectTreeNode( parent )
   , m_projectNodeName( nodeName )
{
   ASSERT_MSG( m_projectNodeName.empty() ? true : m_projectNodeName.c_str()[ m_projectNodeName.length() - 1 ] == '/', "This is a directory node, thus it has to end with a slash" );

   // strip the name of the last slash
   if ( !m_projectNodeName.empty() )
   {
      m_projectNodeName = m_projectNodeName.substr( 0, m_projectNodeName.length() - 1 );
   }

   // set the icon
   Filesystem& fs = ResourcesManager::getInstance().getFilesystem();
   QString iconsDir = fs.getShortcut( "editorIcons" ).c_str();
   setIcon( 0, QIcon( iconsDir + "dirIcon.png" ) );

   // set the description
   setText( 0, m_projectNodeName.c_str() );
   setText( 1, "DIR" );
}

///////////////////////////////////////////////////////////////////////////////

std::string ProjectDirNode::getRelativePath() const
{
   ASSERT_MSG ( parent(), "Directory node has to have a parent" );

   std::string path = dynamic_cast< ProjectTreeNode* >( parent() )->getRelativePath();
   path += m_projectNodeName + "/";

   return path;
}

///////////////////////////////////////////////////////////////////////////////

TreeWidgetDescFactory* ProjectDirNode::getDescFactory( ProjectTree& tree )
{
   return tree.getDescFactory();
}

///////////////////////////////////////////////////////////////////////////////

void ProjectDirNode::addNode( unsigned int typeIdx, ProjectTree& tree )
{
   std::string path = getRelativePath();
   return tree.addNode( typeIdx, path );
}

///////////////////////////////////////////////////////////////////////////////

void ProjectDirNode::openItem( ProjectTree& tree )
{
   std::string thisDir = getRelativePath();
   tree.refresh( thisDir );
}

///////////////////////////////////////////////////////////////////////////////

bool ProjectDirNode::compareNodeName( const std::string& name ) const
{
   return name == m_projectNodeName;
}

///////////////////////////////////////////////////////////////////////////////
