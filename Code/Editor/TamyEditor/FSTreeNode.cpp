#include "FSTreeNode.h"
#include "core\Filesystem.h"
#include "core\FilePath.h"


///////////////////////////////////////////////////////////////////////////////

FSTreeNode::FSTreeNode( FSTreeNode* parent, const std::string& nodeName, const Filesystem& fs )
: QTreeWidgetItem( parent )
, m_fsNodeName( nodeName )
, m_fs( fs )
{
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode::FSTreeNode( QTreeWidget* parent, const std::string& nodeName, const Filesystem& fs )
: QTreeWidgetItem( parent )
, m_fsNodeName( nodeName )
, m_fs( fs )
{
}

///////////////////////////////////////////////////////////////////////////////

FSTreeNode* FSTreeNode::find( const std::string& nodeName )
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      FSTreeNode* entry = dynamic_cast< FSTreeNode* >( child( i ) );
      if ( entry && entry->m_fsNodeName == nodeName )
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

   m_fs.remove( path );
   removeChild( child );
}

///////////////////////////////////////////////////////////////////////////////

void FSTreeNode::clearNodes()
{
   int count = childCount();
   for( int i = 0; i < count; ++i )
   {
      removeNode( dynamic_cast< FSTreeNode* >( child( 0 ) ) );
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
