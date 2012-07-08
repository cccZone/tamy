#include "ProjectTreeNode.h"
#include "core\ResourcesManager.h"
#include "core\Filesystem.h"
#include "core\FilePath.h"


///////////////////////////////////////////////////////////////////////////////

ProjectTreeNode::ProjectTreeNode( ProjectTreeNode* parent )
   : QTreeWidgetItem( parent )
{
}

///////////////////////////////////////////////////////////////////////////////

ProjectTreeNode::ProjectTreeNode( QTreeWidget* parent )
   : QTreeWidgetItem( parent )
{
}

///////////////////////////////////////////////////////////////////////////////

ProjectTreeNode* ProjectTreeNode::find( const std::string& nodeName )
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      ProjectTreeNode* entry = static_cast< ProjectTreeNode* >( child( i ) );
      if ( entry && entry->compareNodeName( nodeName ) )
      {
         return entry;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void ProjectTreeNode::removeNode( ProjectTreeNode* child )
{
   FilePath path( child->getRelativePath() );

   ResourcesManager::getInstance().remove( path );
   removeChild( child );
}

///////////////////////////////////////////////////////////////////////////////

void ProjectTreeNode::clearNodes()
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      removeNode( static_cast< ProjectTreeNode* >( child( 0 ) ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void ProjectTreeNode::clear()
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      removeChild( child( 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
