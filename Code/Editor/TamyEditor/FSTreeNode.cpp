#include "FSTreeNode.h"
#include "core\ResourcesManager.h"
#include "core\Filesystem.h"
#include "core\FilePath.h"


///////////////////////////////////////////////////////////////////////////////

FSTreeNode::FSTreeNode( FSTreeNode* parent )
: QTreeWidgetItem( parent )
{
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode::FSTreeNode( QTreeWidget* parent )
: QTreeWidgetItem( parent )
{
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode* FSTreeNode::find( const std::string& nodeName )
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      FSTreeNode* entry = static_cast< FSTreeNode* >( child( i ) );
      if ( entry && entry->compareNodeName( nodeName ) )
      {
         return entry;
      }
   }

   return NULL;
}

///////////////////////////////////////////////////////////////////////////////

void FSTreeNode::removeNode( FSTreeNode* child )
{
   FilePath path( child->getRelativePath() );

   ResourcesManager::getInstance().remove( path );
   removeChild( child );
}

///////////////////////////////////////////////////////////////////////////////

void FSTreeNode::clearNodes()
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      removeNode( static_cast< FSTreeNode* >( child( 0 ) ) );
   }
}

///////////////////////////////////////////////////////////////////////////////

void FSTreeNode::clear()
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      removeChild( child( 0 ) );
   }
}

///////////////////////////////////////////////////////////////////////////////
